/***************************************************************************
 *   Robotic Activities Erlangen e.V.                                      *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   All rights reserved.                                                  *
 *                                                                         *
 ***************************************************************************/

#include "ssl_referee.h"

/*!
 * \brief Initializes team info with default values
 * \param teamInfo The team info to initialize
 */
void teamInfoSetDefault(SSL_Referee::TeamInfo *teamInfo)
{
    teamInfo->set_name("");
    teamInfo->set_score(0);
    teamInfo->set_red_cards(0);
    teamInfo->set_yellow_cards(0);
    teamInfo->set_timeouts(4);
    teamInfo->set_timeout_time(5 * 60 * 1000*1000);
    teamInfo->set_goalie(0);
    assert(teamInfo->IsInitialized());
}
