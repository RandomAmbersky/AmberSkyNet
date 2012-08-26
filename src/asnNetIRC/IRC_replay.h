#ifndef _NH_IRC_REPLAY_H
#define _NH_IRC_REPLAY_H

#include <string>
#include <map>

std::map<std::string,std::string> irc_cd;

void FillReplay(){

    irc_cd.clear();

/////////////////  “”“ œ≈–≈¬Œƒ —ŒŒ¡Ÿ≈Õ»… IRC   ≈ƒ»ÕŒÃ” ‘Œ–Ã¿“” —≈“»
//// œ≈–≈ƒ”Ã¿À. œ”—“‹ ”∆ “¿  ¡”ƒ≈“ :)
//// irc_cd["PART"]="LEAVE";  // ÓÒÚ‡‚ÎˇÂÏ ÎËÌË˛
///////////////// “”“ «¿ ŒÕ◊»À» œ≈–≈¬Œƒ

irc_cd["381"]="RPL_YOUREOPER";       
//            ":You are now an IRC operator"
irc_cd["491"]="ERR_NOOPERHOST";
//            ":No O-lines for your host"
irc_cd["376"]="RPL_ENDOFMOTD";
//            ":End of MOTD command"
irc_cd["372"]="RPL_MOTD";
//            ":- <text>"
irc_cd["375"]="RPL_MOTDSTART";
//           ":- <server> Message of the day - "
//_cd["302"]="RPL_USERHOST";
//c_cd["301"]="RPL_AWAY";
//c_cd["263"]="RPL_TRYAGAIN";
irc_cd["255"]="RPL_LUSERME";
//              ":I have <integer> clients and <integer> \
//                servers"
irc_cd["254"]="RPL_LUSERCHANNELS";
//              "<integer> :channels formed"
irc_cd["253"]="RPL_LUSERUNKNOWN";
//              "<integer> :unknown connection(s)"
irc_cd["252"]="RPL_LUSEROP";
//              "<integer> :operator(s) online"
irc_cd["251"]="RPL_LUSERCLIENT"; 
//              ":There are <integer> users and <integer> \
//               services on <integer> servers"
irc_cd["005"]="RPL_BOUNCE";
//              "Try server <server name>, port <port number>"
irc_cd["004"]="RPL_MYINFO";
//              "<servername> <version> <available user modes> \
//               <available channel modes>"
irc_cd["003"]="RPL_CREATED";
//              "This server was created <date>"
irc_cd["002"]="RPL_YOURHOST";
//              "Your host is <servername>, running version <ver>"
irc_cd["001"]="RPL_WELCOME";
//               "Welcome to the Internet Relay Network \
//               <nick>!<user>@<host>"


//RFC 2812          Internet Relay Chat: Client Protocol        April 2000

irc_cd["001"]="RPL_WELCOME";
//              "Welcome to the Internet Relay Network \
//               <nick>!<user>@<host>"
irc_cd["002"]="RPL_YOURHOST";
//              "Your host is <servername>, running version <ver>"
irc_cd["003"]="RPL_CREATED";

//              "This server was created <date>"
irc_cd["004"]="RPL_MYINFO";

//              "<servername> <version> <available user modes> \
//               <available channel modes>"
//         - The server sends Replies 001 to 004 to a user upon
//           successful registration.
irc_cd["005"]="RPL_BOUNCE";

//              "Try server <server name>, port <port number>"

//         - Sent by the server to a user to suggest an alternative
//           server.  This is often used when the connection is
//           refused because the server is already full.
irc_cd["302"]="RPL_USERHOST";

//              ":*1<reply> *( " " <reply> )"

//         - Reply format used by USERHOST to list replies to
//           the query list.  The reply string is composed as
//           follows:

//           reply = <nickname> [ "*" ] "=" ( "+" / "-" ) <hostname>

//           The '*' indicates whether the client has registered
//           as an Operator.  The '-' or '+' characters represent
//           whether the client has set an AWAY message or not
//           respectively.
irc_cd["303"]="RPL_ISON";

//              ":*1<nick> *( " " <nick> )"

//         - Reply format used by ISON to list replies to the
//           query list.
irc_cd["301"]="RPL_AWAY";

  //            "<nick> :<away message>"
irc_cd["305"]="RPL_UNAWAY";

//              ":You are no longer marked as being away"
//irc_cd["306"]="RPL_NOWAWAY";

//              ":You have been marked as being away"

//         - These replies are used with the AWAY command (if
//           allowed).  RPL_AWAY is sent to any client sending a
//           PRIVMSG to a client which is away.  RPL_AWAY is only
//           sent by the server to which the client is connected.
//           Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
//           client removes and sets an AWAY message.
irc_cd["311"]="RPL_WHOISUSER";

//              "<nick> <user> <host> * :<real name>"
irc_cd["312"]="RPL_WHOISSERVER";

//              "<nick> <server> :<server info>"
irc_cd["313"]="RPL_WHOISOPERATOR";

//              "<nick> :is an IRC operator"
irc_cd["317"]="RPL_WHOISIDLE";

//              "<nick> <integer> :seconds idle"
irc_cd["318"]="RPL_ENDOFWHOIS";

//              "<nick> :End of WHOIS list"
irc_cd["319"]="RPL_WHOISCHANNELS";

//              "<nick> :*( ( "@" / "+" ) <channel> " " )"
irc_cd["314"]="RPL_WHOWASUSER";

//              "<nick> <user> <host> * :<real name>"
irc_cd["369"]="RPL_ENDOFWHOWAS";

//              "<nick> :End of WHOWAS"
irc_cd["321"]="RPL_LISTSTART";

//              Obsolete. Not used.
irc_cd["322"]="RPL_LIST";

//              "<channel> <# visible> :<topic>"
irc_cd["323"]="RPL_LISTEND";

//              ":End of LIST"
irc_cd["325"]="RPL_UNIQOPIS";

//              "<channel> <nickname>"
irc_cd["324"]="RPL_CHANNELMODEIS";

//              "<channel> <mode> <mode params>"
irc_cd["331"]="RPL_NOTOPIC";

//              "<channel> :No topic is set"
irc_cd["332"]="RPL_TOPIC";

//              "<channel> :<topic>"
irc_cd["341"]="RPL_INVITING";

//              "<channel> <nick>"
irc_cd["342"]="RPL_SUMMONING";

//              "<user> :Summoning user to IRC"
irc_cd["346"]="RPL_INVITELIST";

//              "<channel> <invitemask>"
irc_cd["347"]="RPL_ENDOFINVITELIST";

//              "<channel> :End of channel invite list"
irc_cd["348"]="RPL_EXCEPTLIST";

//            "<channel> <exceptionmask>"
irc_cd["349"]="RPL_ENDOFEXCEPTLIST";

//              "<channel> :End of channel exception list"
irc_cd["351"]="RPL_VERSION";

//            "<version>.<debuglevel> <server> :<comments>"
irc_cd["352"]="RPL_WHOREPLY";

//          "<channel> <user> <host> <server> <nick> \
//               <H|G>[*][@|+] :<hopcount> <real name>"
irc_cd["315"]="RPL_ENDOFWHO";

//              "<name> :End of WHO list"
irc_cd["353"]="RPL_NAMREPLY";

//              "( "=" / "*" / "@" ) <channel> \
//               :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
//         - "@" is used for secret channels, "*" for private
//           channels, and "=" for others (public channels).
irc_cd["366"]="RPL_ENDOFNAMES";

//              "<channel> :End of NAMES list"
irc_cd["364"]="RPL_LINKS";

//              "<mask> <server> :<hopcount> <server info>"
irc_cd["365"]="RPL_ENDOFLINKS";

//              "<mask> :End of LINKS list"
irc_cd["367"]="RPL_BANLIST";

//              "<channel> <banmask>"
irc_cd["368"]="RPL_ENDOFBANLIST";

//              "<channel> :End of channel ban list"
irc_cd["371"]="RPL_INFO";

//              ":<string>"
irc_cd["374"]="RPL_ENDOFINFO";

//              ":End of INFO list"
irc_cd["375"]="RPL_MOTDSTART";

//              ":- <server> Message of the day - "
irc_cd["372"]="RPL_MOTD";

//              ":- <text>"
irc_cd["376"]="RPL_ENDOFMOTD";

//              ":End of MOTD command"
irc_cd["381"]="RPL_YOUREOPER";

//              ":You are now an IRC operator"

//       - RPL_YOUREOPER is sent back to a client which has
//           just successfully issued an OPER message and gained
//           operator status.
irc_cd["382"]="RPL_REHASHING";

//              "<config file> :Rehashing"

//         - If the REHASH option is used and an operator sends
//           a REHASH message, an RPL_REHASHING is sent back to
//           the operator.
irc_cd["383"]="RPL_YOURESERVICE";

//              "You are service <servicename>"

//         - Sent by the server to a service upon successful
//           registration.
irc_cd["391"]="RPL_TIME";

//              "<server> :<string showing server's local time>"

//         - When replying to the TIME message, a server MUST send
//           the reply using the RPL_TIME format above.  The string
//           showing the time need only contain the correct day and
//           time there.  There is no further requirement for the
//           time string.
irc_cd["392"]="RPL_USERSSTART";

//              ":UserID   Terminal  Host"
irc_cd["393"]="RPL_USERS";

//              ":<username> <ttyline> <hostname>"
irc_cd["394"]="RPL_ENDOFUSERS";

//              ":End of users"
irc_cd["395"]="RPL_NOUSERS";

//              ":Nobody logged in"

//         - If the USERS message is handled by a server, the
//           replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and
//           RPL_NOUSERS are used.  RPL_USERSSTART MUST be sent
//           first, following by either a sequence of RPL_USERS
//           or a single RPL_NOUSER.  Following this is
//           RPL_ENDOFUSERS.
irc_cd["200"]="RPL_TRACELINK";

//              "Link <version & debug level> <destination> \
//               <next server> V<protocol version> \
//               <link uptime in seconds> <backstream sendq>\
//               <upstream sendq>"
irc_cd["201"]="RPL_TRACECONNECTING";

//              "Try. <class> <server>"
irc_cd["202"]="RPL_TRACEHANDSHAKE";

//              "H.S. <class> <server>"
irc_cd["203"]="RPL_TRACEUNKNOWN";

//              "???? <class> [<client IP address in dot form>]"
irc_cd["204"]="RPL_TRACEOPERATOR";

//              "Oper <class> <nick>"
irc_cd["205"]="RPL_TRACEUSER";

//              "User <class> <nick>"
irc_cd["206"]="RPL_TRACESERVER";

//              "Serv <class> <int>S <int>C <server> \
//               <nick!user|*!*>@<host|server> V<protocol version>"
irc_cd["207"]="RPL_TRACESERVICE";

//              "Service <class> <name> <type> <active type>"
irc_cd["208"]="RPL_TRACENEWTYPE";

//              "<newtype> 0 <client name>"
irc_cd["209"]="RPL_TRACECLASS";

//              "Class <class> <count>"
irc_cd["210"]="RPL_TRACERECONNECT";

//              Unused.
irc_cd["261"]="RPL_TRACELOG";

//              "File <logfile> <debug level>"
irc_cd["262"]="RPL_TRACEEND";

//              "<server name> <version & debug level> :End of TRACE"

//         - The RPL_TRACE* are all returned by the server in
//           response to the TRACE message.  How many are
//           returned is dependent on the TRACE message and
//           whether it was sent by an operator or not.  There
//           is no predefined order for which occurs first.
//           Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and
//           RPL_TRACEHANDSHAKE are all used for connections
//           which have not been fully established and are either
//           unknown, still attempting to connect or in the
//           process of completing the 'server handshake'.
//           RPL_TRACELINK is sent by any server which handles
//           a TRACE message and has to pass it on to another
//           server.  The list of RPL_TRACELINKs sent in
//           response to a TRACE command traversing the IRC
//           network should reflect the actual connectivity of
//           the servers themselves along that path.

//           RPL_TRACENEWTYPE is to be used for any connection
//           which does not fit in the other categories but is
//           being displayed anyway.
//           RPL_TRACEEND is sent to indicate the end of the list.
irc_cd["211"]="RPL_STATSLINKINFO";

//              "<linkname> <sendq> <sent messages> \
//               <sent Kbytes> <received messages> \
//               <received Kbytes> <time open>"

//         - reports statistics on a connection.  <linkname>
//           identifies the particular connection, <sendq> is
//           the amount of data that is queued and waiting to be
//           sent <sent messages> the number of messages sent,
//           and <sent Kbytes> the amount of data sent, in
//           Kbytes. <received messages> and <received Kbytes>
//           are the equivalent of <sent messages> and <sent
//           Kbytes> for received data, respectively.  <time
//           open> indicates how long ago the connection was
//           opened, RPL_STATSCOMMANDS seconds.
irc_cd["212"]="RPL_YOURHOST";

//              "<command> <count> <byte count> <remote count>"

//         - reports statistics on commands usage.
irc_cd["219"]="RPL_ENDOFSTATS";

//              "<stats letter> :End of STATS report"
irc_cd["242"]="RPL_STATSUPTIME";

//              ":Server Up %d days %d:%02d:%02d"
//         - reports the server uptime.
irc_cd["243"]="RPL_STATSOLINE";

//              "O <hostmask> * <name>"

//         - reports the allowed hosts from where user may become IRC
//            operators.
irc_cd["221"]="RPL_UMODEIS";
//              "<user mode string>"

//         - To answer a query about a client's own mode,
//           RPL_UMODEIS is sent back.
irc_cd["234"]="RPL_SERVLIST";

  //            "<name> <server> <mask> <type> <hopcount> <info>"
irc_cd["235"]="RPL_SERVLISTEND";

//              "<mask> <type> :End of service listing"

//         - When listing services in reply to a SERVLIST message,
//           a server is required to send the list back using the
//           RPL_SERVLIST and RPL_SERVLISTEND messages.  A separate
//           RPL_SERVLIST is sent for each service.  After the
//           services have been listed (or if none present) a
//           RPL_SERVLISTEND MUST be sent.
irc_cd["251"]="RPL_LUSERCLIENT";

//              ":There are <integer> users and <integer> \
//               services on <integer> servers"
irc_cd["252"]="RPL_LUSEROP";

//              "<integer> :operator(s) online"
irc_cd["253"]="RPL_LUSERUNKNOWN";

//              "<integer> :unknown connection(s)"
irc_cd["254"]="RPL_LUSERCHANNELS";

//              "<integer> :channels formed"
irc_cd["255"]="RPL_LUSERME";

//              ":I have <integer> clients and <integer> \
//                servers"

//         - In processing an LUSERS message, the server
//           sends a set of replies from RPL_LUSERCLIENT,
//           RPL_LUSEROP, RPL_USERUNKNOWN,
//           RPL_LUSERCHANNELS and RPL_LUSERME.  When
//           replying, a server MUST send back
//           RPL_LUSERCLIENT and RPL_LUSERME.  The other
//           replies are only sent back if a non-zero count
//           is found for them.
irc_cd["256"]="RPL_ADMINME";

//              "<server> :Administrative info"
irc_cd["257"]="RPL_ADMINLOC1";

//              ":<admin info>"
irc_cd["258"]="RPL_ADMINLOC2";

//              ":<admin info>"
irc_cd["259"]="RPL_ADMINEMAIL";

//              ":<admin info>"

  //       - When replying to an ADMIN message, a server
//           is expected to use replies RPL_ADMINME
//           through to RPL_ADMINEMAIL and provide a text
//           message with each.  For RPL_ADMINLOC1 a
//           description of what city, state and country
//           the server is in is expected, followed by
//           details of the institution (RPL_ADMINLOC2)
//           and finally the administrative contact for the
//           server (an email address here is REQUIRED)
//           in RPL_ADMINEMAIL.
irc_cd["263"]="RPL_TRYAGAIN";

//              "<command> :Please wait a while and try again."

//         - When a server drops a command without processing it,
//           it MUST use the reply RPL_TRYAGAIN to inform the
//           originating client.
irc_cd["401"]="ERR_NOSUCHNICK";

//              "<nickname> :No such nick/channel"

//          - Used to indicate the nickname parameter supplied to a
//            command is currently unused.
irc_cd["402"]="ERR_NOSUCHSERVER";

//            "<server name> :No such server"

//         - Used to indicate the server name given currently
//           does not exist.
irc_cd["403"]="ERR_NOSUCHCHANNEL";

//              "<channel name> :No such channel"
//         - Used to indicate the given channel name is invalid.
irc_cd["404"]="ERR_CANNOTSENDTOCHAN";

//              "<channel name> :Cannot send to channel"

//         - Sent to a user who is either (a) not on a channel
//           which is mode +n or (b) not a chanop (or mode +v) on
//           a channel which has mode +m set or where the user is
//           banned and is trying to send a PRIVMSG message to
//           that channel.
irc_cd["405"]="ERR_TOOMANYCHANNELS";

//              "<channel name> :You have joined too many channels"

//         - Sent to a user when they have joined the maximum
//           number of allowed channels and they try to join
//           another channel.
irc_cd["406"]="ERR_WASNOSUCHNICK";

//              "<nickname> :There was no such nickname"

//         - Returned by WHOWAS to indicate there is no history
//           information for that nickname.
irc_cd["407"]="ERR_TOOMANYTARGETS";

//              "<target> :<error code> recipients. <abort message>"

//         - Returned to a client which is attempting to send a
//           PRIVMSG/NOTICE using the user@host destination format
//           and for a user@host which has several occurrences.

//         - Returned to a client which trying to send a
//           PRIVMSG/NOTICE to too many recipients.

//         - Returned to a client which is attempting to JOIN a safe
//           channel using the shortname when there are more than one
//           such channel.
irc_cd["408"]="ERR_NOSUCHSERVICE";

//              "<service name> :No such service"

//         - Returned to a client which is attempting to send a SQUERY
//           to a service which does not exist.
irc_cd["409"]="ERR_NOORIGIN";

//              ":No origin specified"

//         - PING or PONG message missing the originator parameter.
irc_cd["411"]="ERR_NORECIPIENT";

//              ":No recipient given (<command>)"
irc_cd["412"]="ERR_NOTEXTTOSEND";

//              ":No text to send"
irc_cd["413"]="ERR_NOTOPLEVEL";

//              "<mask> :No toplevel domain specified"
irc_cd["414"]="ERR_WILDTOPLEVEL";

//              "<mask> :Wildcard in toplevel domain"
irc_cd["415"]="ERR_BADMASK";

//              "<mask> :Bad Server/host mask"

  //       - 412 - 415 are returned by PRIVMSG to indicate that
//           the message wasn't delivered for some reason.
//           ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
//           are returned when an invalid use of
//           "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
irc_cd["421"]="ERR_UNKNOWNCOMMAND";

//              "<command> :Unknown command"

//         - Returned to a registered client to indicate that the
//           command sent is unknown by the server.
irc_cd["422"]="ERR_NOMOTD";

//              ":MOTD File is missing"

//         - Server's MOTD file could not be opened by the server.
irc_cd["423"]="ERR_NOADMININFO";

//              "<server> :No administrative info available"

//         - Returned by a server in response to an ADMIN message
//           when there is an error in finding the appropriate
//           information.
irc_cd["424"]="ERR_FILEERROR";

//              ":File error doing <file op> on <file>"

//         - Generic error message used to report a failed file
//           operation during the processing of a message.
irc_cd["431"]="ERR_NONICKNAMEGIVEN";

//              ":No nickname given"

//         - Returned when a nickname parameter expected for a
//           command and isn't found.
irc_cd["432"]="ERR_ERRONEUSNICKNAME";

//              "<nick> :Erroneous nickname"

//         - Returned after receiving a NICK message which contains
//           characters which do not fall in the defined set.  See
//           section 2.3.1 for details on valid nicknames.
irc_cd["433"]="ERR_NICKNAMEINUSE";

//              "<nick> :Nickname is already in use"

//         - Returned when a NICK message is processed that results
//           in an attempt to change to a currently existing
//           nickname.
irc_cd["436"]="ERR_NICKCOLLISION";

//              "<nick> :Nickname collision KILL from <user>@<host>"

//         - Returned by a server to a client when it detects a
//           nickname collision (registered of a NICK that
//           already exists by another server).
irc_cd["437"]="ERR_UNAVAILRESOURCE";

//              "<nick/channel> :Nick/channel is temporarily unavailable"

//         - Returned by a server to a user trying to join a channel
//           currently blocked by the channel delay mechanism.

//         - Returned by a server to a user trying to change nickname
//           when the desired nickname is blocked by the nick delay
//           mechanism.
irc_cd["441"]="ERR_USERNOTINCHANNEL";

//              "<nick> <channel> :They aren't on that channel"

//         - Returned by the server to indicate that the target
//           user of the command is not on the given channel.
irc_cd["442"]="ERR_NOTONCHANNEL";

//              "<channel> :You're not on that channel"

//         - Returned by the server whenever a client tries to
//           perform a channel affecting command for which the
//           client isn't a member.
irc_cd["443"]="ERR_USERONCHANNEL";

//              "<user> <channel> :is already on channel"

//         - Returned when a client tries to invite a user to a
//           channel they are already on.
irc_cd["444"]="ERR_NOLOGIN";

//              "<user> :User not logged in"

//         - Returned by the summon after a SUMMON command for a
//           user was unable to be performed since they were not
//           logged in.
irc_cd["445"]="ERR_SUMMONDISABLED";

//              ":SUMMON has been disabled"

//         - Returned as a response to the SUMMON command.  MUST be
//           returned by any server which doesn't implement it.
irc_cd["446"]="ERR_USERSDISABLED";

//              ":USERS has been disabled"

//         - Returned as a response to the USERS command.  MUST be
//           returned by any server which does not implement it.
irc_cd["451"]="ERR_NOTREGISTERED";

//              ":You have not registered"

//         - Returned by the server to indicate that the client
//           MUST be registered before the server will allow it
//           to be parsed in detail.
irc_cd["461"]="ERR_NEEDMOREPARAMS";

//              "<command> :Not enough parameters"

//         - Returned by the server by numerous commands to
//           indicate to the client that it didn't supply enough
//           parameters.
irc_cd["462"]="ERR_ALREADYREGISTRED";

//              ":Unauthorized command (already registered)"

//         - Returned by the server to any link which tries to
//           change part of the registered details (such as
//           password or user details from second USER message).
irc_cd["463"]="ERR_NOPERMFORHOST";

//              ":Your host isn't among the privileged"

//         - Returned to a client which attempts to register with
//           a server which does not been setup to allow
//           connections from the host the attempted connection
//           is tried.
irc_cd["464"]="ERR_PASSWDMISMATCH";

//              ":Password incorrect"

//         - Returned to indicate a failed attempt at registering
//           a connection for which a password was required and
//           was either not given or incorrect.
irc_cd["465"]="ERR_YOUREBANNEDCREEP";

//              ":You are banned from this server"

//         - Returned after an attempt to connect and register
//           yourself with a server which has been setup to
//           explicitly deny connections to you.
irc_cd["466"]="ERR_YOUWILLBEBANNED";


//         - Sent by a server to a user to inform that access to the
//           server will soon be denied.
irc_cd["467"]="ERR_KEYSET";

//              "<channel> :Channel key already set"
irc_cd["471"]="ERR_CHANNELISFULL";

//              "<channel> :Cannot join channel (+l)"
irc_cd["472"]="ERR_UNKNOWNMODE";

//              "<char> :is unknown mode char to me for <channel>"
//irc_cd["473"]="ERR_INVITEONLYCHAN";

//              "<channel> :Cannot join channel (+i)"
//irc_cd["474"]="ERR_BANNEDFROMCHAN";

//              "<channel> :Cannot join channel (+b)"
//irc_cd["475"]="ERR_BADCHANNELKEY";

//              "<channel> :Cannot join channel (+k)"
//irc_cd["476"]="ERR_BADCHANMASK";

//              "<channel> :Bad Channel Mask"
//irc_cd["477"]="ERR_NOCHANMODES";

//              "<channel> :Channel doesn't support modes"
//irc_cd["478"]="ERR_BANLISTFULL";

//              "<channel> <char> :Channel list is full"
//irc_cd["481"]="ERR_NOPRIVILEGES";

//              ":Permission Denied- You're not an IRC operator"

//         - Any command requiring operator privileges to operate
//           MUST return this error to indicate the attempt was
//           unsuccessful.
//irc_cd["482"]="ERR_CHANOPRIVSNEEDED";

//              "<channel> :You're not channel operator"

//         - Any command requiring 'chanop' privileges (such as

//           MODE messages) MUST return this error if the client
//           making the attempt is not a chanop on the specified
//           channel.
//irc_cd["483"]="ERR_CANTKILLSERVER";

//              ":You can't kill a server!"

//         - Any attempts to use the KILL command on a server
//           are to be refused and this error returned directly
//           to the client.
//irc_cd["484"]="ERR_RESTRICTED";

//              ":Your connection is restricted!"

//         - Sent by the server to a user upon connection to indicate
//           the restricted nature of the connection (user mode "+r").
//irc_cd["485"]="ERR_UNIQOPPRIVSNEEDED";

//              ":You're not the original channel operator"

//         - Any MODE requiring "channel creator" privileges MUST
//           return this error if the client making the attempt is not
//           a chanop on the specified channel.
//irc_cd["491"]="ERR_NOOPERHOST";

//              ":No O-lines for your host"

//         - If a client sends an OPER message and the server has
//           not been configured to allow connections from the
//           client's host as an operator, this error MUST be
//           returned.
//irc_cd["501"]="ERR_UMODEUNKNOWNFLAG";

//              ":Unknown MODE flag"

//         - Returned by the server to indicate that a MODE
//           message was sent with a nickname parameter and that
//           the a mode flag sent was not recognized.
//irc_cd["502"]="ERR_USERSDONTMATCH";

//              ":Cant change mode for other users"

//         - Error sent to any user trying to view or change the
//           user mode for a user other than themselves.

//   These numerics are not described above since they fall into one of
//   the following categories:

//   1. no longer in use;
//   2. reserved for future planned use;
//   3. in current use but are part of a non-generic 'feature' of
//      the current IRC server.
/*
            231    RPL_SERVICEINFO     232  RPL_ENDOFSERVICES
            233    RPL_SERVICE
            300    RPL_NONE            316  RPL_WHOISCHANOP
            361    RPL_KILLDONE        362    RPL_CLOSING
            363    RPL_CLOSEEND        373  RPL_INFOSTART
            384    RPL_MYPORTIS

            213    RPL_STATSCLINE      214  RPL_STATSNLINE
            215    RPL_STATSILINE      216  RPL_STATSKLINE
            217    RPL_STATSQLINE      218  RPL_STATSYLINE
            240    RPL_STATSVLINE      241  RPL_STATSLLINE
            244    RPL_STATSHLINE      244  RPL_STATSSLINE
            246    RPL_STATSPING       247  RPL_STATSBLINE
            250    RPL_STATSDLINE

            492    ERR_NOSERVICEHOST
*/

}

#endif
