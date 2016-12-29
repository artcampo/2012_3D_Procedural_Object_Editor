#include	"Editor_Texture/DataLayer/Channel.hpp"

//===========================================================================
/*!
    Constructor
    \param    aNewIdentifiers   id's object
*/
//===========================================================================

ChannelRgba::ChannelRgba( Identifiers* aNewIdentifiers, const int aChannelId, const string& aChannelName )
  : Channel( aNewIdentifiers, aChannelId, aChannelName )
{
}
