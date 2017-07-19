/**
 * \struct noteMessage
 *
 *
 * \brief Structure for holding Note (on,off) messages.
 *
 *
 * \author Henrik von Coler
 *
 * \version $Revision: 0.5 $
 *
 * \date $Date: 2005/04/14 14:16:20 $
 *
 * Contact: von_coler@intelligent-noise-solutions.de
 *
 *
 */

#pragma once


typedef struct  {
    int noteNumber          = -1;    /// @var noteNumber The note number
    double velocity         = -1;
    bool hasBeenProcessed   = false;

}noteMessage;


typedef struct  {
    int ctlNumber          = -1;    /// @var noteNumber The note number
    double value         = -1;
    bool hasBeenProcessed   = false;

}ctlMessage;

