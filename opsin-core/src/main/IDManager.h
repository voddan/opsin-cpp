#pragma once


/// <summary>
///A source of unique integers. Starts at 1 by default.
///
/// @author ptc24
///
/// </summary>
class IDManager {
    /// <summary>
    ///the last integer generated, or 0 at first </summary>
private:
    int currentID = 0;

public:
    virtual int getCurrentID();

    /// <summary>
    ///Initialises currentID at zero - will give 1 when first called </summary>
    IDManager();

    /// <summary>
    ///Generates a new, unique integer. This is one
    /// higher than the previous integer, or 1 if previously uncalled. </summary>
    /// <returns> The generated integer. </returns>
    virtual int getNextID();

};

