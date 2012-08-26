#include "nh_jabber.h"
#include "IEngine.h"
#include "ILog.h"
#include "INetSupervisor.h"
#include "base64.h"
#include "md5.h"
#include "iconv.h"
#include <stdlib.h>
#include <iostream>
#define ENGINE Engine //у нас уже есть указатель на Engine

using namespace std;

// based on some code of gloox library ( http://camaya.net/gloox )
// clientbase.cpp

char* convert(const char* s, const char* from_cp, const char* to_cp)
{
iconv_t ic = iconv_open(to_cp, from_cp);

if (ic == (iconv_t)(-1)) {
fprintf(stderr, "iconv: cannot convert from %s to %s\n", from_cp, to_cp);
return "";
}

char* out_buf = (char*)calloc(strlen(s)+1, 1);
char* out = out_buf;
char* in = (char*)malloc(strlen(s)+1);
strcpy(in, s);

size_t in_ln = strlen(s);
size_t out_ln = in_ln;
size_t k = iconv(ic, &in, &in_ln, &out, &out_ln);
if(k == (size_t)-1)
fprintf(stderr, "iconv: %u of %d converted\n", k, strlen(s));

//if(errno != 0)
//fprintf(stderr, "iconv: %s\n", strerror(errno));

iconv_close(ic);

return out_buf;
}
//char* s = convert(postutf8, "utf-8", "cp1251");
std::string utf8_conv_rand(std::string base, std::string From="utf-8", std::string To="koi8-r"){


	char* ret_s = convert( base.c_str(), From.c_str(), To.c_str() );	
	std::string ret = ret_s;
	free(ret_s);
	return ret;

}

NH_Jabber::NH_Jabber(IEngine *_Engine):CBaseObject( _Engine ){

	SetParam("BaseClass","NetProtocolHandler");
	SetParam("Type","Jabber_Client");
	SetParam("Name","-");

	SetParam("UserName","test");
	SetParam("ServerName","test");
	SetParam("Password","test");

//	TryConnect = 0;
	LOGGER->LogMsg("Jabber_Client: Open");
//	((ILog*)_Engine->GetPtrParam("LogManager"))->LogMsg("Jabber_Client: Open");
	connected=false;
	cut_buffer="";

};

NH_Jabber::~NH_Jabber(){


};


void NH_Jabber::Init(const char* Init_String){

	LOGGER->LogMsg("Jabber_Client init...");

};


bool NH_Jabber::Send(const char* Operation, const char* To, const char* Value){

std::string send;

if ( ASN_Str(To)=="server" ) {

	if ( ASN_Str(Operation)=="connect" ) {
		send = "";//<?xml version='1.0' encoding='cp1251'?>";
		send = send + "<stream:stream to='"+GetParam("ServerName")+"' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams'  xml:l='ru' version='1.0'>";
//		NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());
		printf("%d <= %s \n", SupervisorId, send.c_str() );
		NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());
//		connected = true;
	}

	else {

		send=ASN_Str(Operation);
		NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());

		}

} // server operations

 
//send=ASN_Str(Operation)+" "+ASN_Str(Value)+"\n";
//    NETTER->SendMsg(SupervisorId,(char *)send.c_str(),send.length());
//    printf("send..\n");
//}


};

// наследство от IBaseObject
void NH_Jabber::SetPtrParam(const char* param_name, void *param_value){


};

unsigned int NH_Jabber::HandleMessage(unsigned int handler_id, char *buffer, long int buf_size){

    printf("jabber handler_beg: %d..\n", handler_id);

std::string AllMessages;
std::string OutString;
int pos, pos2;

if (buffer) { // 
buffer[buf_size]='\0';
AllMessages=buffer;
//AllMessages=cut_buffer+buffer;
}
else AllMessages=cut_buffer;

AllMessages = utf8_conv_rand( AllMessages );
cout << AllMessages.c_str() << endl;

//if (buffer) { // 
//buffer[buf_size]='\0';
//cout << buffer << endl;
//}

//else 
//if ( handler_id==0 )  {
//    handler_id = 1;
//}
//else 
if ( handler_id==0 ){

	if (!connected) {
	std::string send="<auth xmlns='urn:ietf:params:xml:ns:xmpp-sasl' mechanism='DIGEST-MD5'/>";
	Send(send.c_str(),"server",NULL);
	handler_id = 1;
	connected = true;
	}

} else if ( handler_id==1 ){

	std::string nonce;
// <challenge xmlns='urn:ietf:params:xml:ns:xmpp-sasl'>...</challenge>	
	pos=AllMessages.find_first_of(">");
	std::string challenge = AllMessages.substr(pos+1);
	pos=challenge.find_first_of("<");
	challenge = challenge.substr(0,pos);

	challenge = Base64::decode64( challenge );
// nonce="22647748",qop="auth",charset=utf-8,algorithm=md5-sess
	cout << challenge.c_str() << endl;

	pos = challenge.find( "nonce=" );
	pos2 = challenge.find( '"', pos + 7 );
	nonce = challenge.substr( pos + 7, pos2 - (pos+7) );
	
	cout << "nonce=" << nonce.c_str() << endl;

	std::string cnonce;
        char cn[4*8+1];
        for( int i = 0; i < 4; ++i )
        sprintf( cn + i*8, "%08x", rand() );
        cnonce.assign( cn, 4*8 );

	cout << "cnonce=" << cnonce.c_str() << endl;

	MD5 md5;
	md5.feed( GetParam("UserName") );
	md5.feed( ":" );
        md5.feed( GetParam("ServerName") );
        md5.feed( ":" );
        md5.feed( GetParam("Password") );
        md5.finalize();
	const std::string& a1_h = md5.binary();

        md5.reset();
        md5.feed( a1_h );
        md5.feed( ":" );
        md5.feed( nonce );
        md5.feed( ":" );
        md5.feed( cnonce );
        md5.finalize();
        const std::string& a1  = md5.hex();

        md5.reset();
        md5.feed( "AUTHENTICATE:xmpp/" );
        md5.feed( GetParam("ServerName") );
        md5.finalize();
        const std::string& a2 = md5.hex();

        md5.reset();
        md5.feed( a1 );
        md5.feed( ":" );
        md5.feed( nonce );
        md5.feed( ":00000001:" );
        md5.feed( cnonce );
        md5.feed( ":auth:" );
        md5.feed( a2 );
        md5.finalize();

	std::string response = "username=\"";
        response += GetParam("UserName");
        response += "\",realm=\"";
        response += GetParam("ServerName");
        response += "\",nonce=\"";
        response += nonce;
        response += "\",cnonce=\"";
        response += cnonce;
        response += "\",nc=00000001,qop=auth,digest-uri=\"xmpp/";
        response += GetParam("ServerName");
        response += "\",response=";
        response += md5.hex();
        response += ",charset=utf-8";

	response = Base64::encode64( response );
	response = "<response xmlns='urn:ietf:params:xml:ns:xmpp-sasl'>"+response+"</response>";

	Send(response.c_str(),"server",NULL);

handler_id = 2;

} else if ( handler_id==2 ){

	std::string send = "<response xmlns='urn:ietf:params:xml:ns:xmpp-sasl'/>";
	Send(send.c_str(),"server",NULL);
	handler_id = 3;

} else if ( handler_id==3 ){

	Send("connect","server","10");
	handler_id = 4;

} else if ( handler_id==4 ){

//   <iq type='set' id='bind_2'>
//     <bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'>
//       <resource>someresource</resource>
//     </bind>
//   </iq> 

	std::string send ="<iq type='set' id='bund_2'><bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'><resource>test</resource></bind></iq>";
	Send(send.c_str(),"server",NULL);

	handler_id = 5;

} else if ( handler_id==5 ){


	std::string send ="<iq to='";
	send = send+GetParam("ServerName")+"' type='set' id='sess_1'><session xmlns='urn:ietf:params:xml:ns:xmpp-session'/></iq>";
	Send(send.c_str(),"server",NULL);

	handler_id = 6;

} else if ( handler_id==6 ){

	std::string send ="<presence><show></show></presence>";
	Send(send.c_str(),"server",NULL);
	
	handler_id = 7;

} else if ( handler_id==7 ){

	std::string send = "<iq from='tes/test' type='get' id='roster_1'><query xmlns='jabber:iq:roster'/></iq>";
	Send(send.c_str(),"server",NULL);

	handler_id = 8;
}

    printf("jabber handler_end: %d..\n", handler_id);
    return handler_id;

};
