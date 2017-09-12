#include "ResourceGetter.h"

namespace uk
{
	namespace ac
	{
		namespace cam
		{
			namespace ch
			{
				namespace wwmm
				{
					namespace opsin
					{
						using org::apache::commons::io::IOUtils;
						using org::codehaus::stax2::XMLInputFactory2;
						using com::ctc::wstx::stax::WstxInputFactory;
javax::xml::stream::XMLInputFactory *const ResourceGetter::xmlInputFactory;

						ResourceGetter::StaticConstructor::StaticConstructor()
						{
															xmlInputFactory = new WstxInputFactory();
															xmlInputFactory->setProperty(XMLInputFactory::SUPPORT_DTD, false);
															xmlInputFactory->setProperty(XMLInputFactory2::P_AUTO_CLOSE_INPUT, true);
						}

ResourceGetter::StaticConstructor ResourceGetter::staticConstructor;

						ResourceGetter::ResourceGetter(const std::wstring &resourcePath) : resourcePath(resourcePath.substr(1)), workingDirectory((&tempVar)->getCanonicalPath()) / * works on linux unlike using the system property */
						{
							if (boost::starts_with(resourcePath, L"/"))
							{
							}
							this->resourcePath = resourcePath;
							std::wstring workingDirectory;
							try
							{
							}
							catch (const IOException &e)
							{
								//Automata will not be serialisable
								workingDirectory = L"";
							}
							this->workingDirectory = workingDirectory;
						}

						std::wstring ResourceGetter::getResourcePath()
						{
							return resourcePath;
						}

						XMLStreamReader *ResourceGetter::getXMLStreamReader(const std::wstring &name) throw(IOException)
						{
							if (name == L"")
							{
								throw std::invalid_argument("Input to function was null");
							}
							try
							{
								if (workingDirectory != L"")
								{
									File *f = getFile(name);
									if (f != nullptr)
									{
										FileInputStream tempVar(f);
										return xmlInputFactory->createXMLStreamReader(&tempVar);
									}
								}
								ClassLoader *l = getClass().getClassLoader();
								URL *url = l->getResource(resourcePath + name);
								if (url == nullptr)
								{
									throw IOException(std::wstring(L"URL for resource: ") + resourcePath + name + std::wstring(L" is invalid"));
								}
								return xmlInputFactory->createXMLStreamReader(url->openStream());
							}
							catch (const XMLStreamException &e)
							{
								throw IOException(std::wstring(L"Validity exception occurred while reading the XML file with name:") + name, e);
							}
						}

						File *ResourceGetter::getFile(const std::wstring &name)
						{
							File *f = new File(getResDir(), name);
							if (f->isFile())
							{
								return f;
							}
							return nullptr;
						}

						File *ResourceGetter::getResDir()
						{
							File *resourcesTop = new File(workingDirectory, L"resources");
							return new File(resourcesTop, resourcePath);
						}

						std::wstring ResourceGetter::getFileContentsAsString(const std::wstring &name)
						{
							if (name == L"")
							{
								throw std::invalid_argument("Input to function was null");
							}
							InputStreamReader *is = nullptr;
							try
							{
								try
								{
									is = new InputStreamReader(getInputstreamFromFileName(name), L"UTF-8");
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
									return IOUtils->toString(is);
								}
								catch (const UnsupportedEncodingException &e)
								{
//JAVA TO C++ CONVERTER TODO TASK: The std::exception constructor has no parameters:
//ORIGINAL LINE: throw new RuntimeException("Java VM is broken; UTF-8 should be supported", e);
									throw std::exception();
								}
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to the exception 'finally' clause:
								finally
								{
									IOUtils::closeQuietly(is);
								}
							}
							catch (const IOException &e)
							{
								return L"";
							}
						}

						InputStream *ResourceGetter::getInputstreamFromFileName(const std::wstring &name) throw(IOException)
						{
							if (name == L"")
							{
								throw std::invalid_argument("Input to function was null");
							}
							if (workingDirectory != L"")
							{
								File *f = getFile(name);
								if (f != nullptr)
								{
									return new FileInputStream(f);
								}
							}
							ClassLoader *l = getClass().getClassLoader();
							URL *url = l->getResource(resourcePath + name);
							if (url == nullptr)
							{
								throw IOException(std::wstring(L"URL for resource: ") + resourcePath + name + std::wstring(L" is invalid"));
							}
							return url->openStream();
						}

						OutputStream *ResourceGetter::getOutputStream(const std::wstring &name) throw(IOException)
						{
							if (name == L"")
							{
								throw std::invalid_argument("Input to function was null");
							}
							File *f = getFileForWriting(name);
							return new FileOutputStream(f);
						}

						File *ResourceGetter::getFileForWriting(const std::wstring &name) throw(IOException)
						{
							File *resourcesTop = new File(workingDirectory, L"resources");
							File *resDir = new File(resourcesTop, resourcePath);
							if (!resDir->exists())
							{
								if (!resDir->mkdirs())
								{
									throw IOException(std::wstring(L"Failed to generate requested directories to create: ") + name);
								}
							}
							return new File(resDir, name);
						}
					}
				}
			}
		}
	}
}
