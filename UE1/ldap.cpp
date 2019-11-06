#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ldap.h>
#include <cstdlib>

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "ClientSocket.hpp"
#include "utils.hpp"

#define LDAP_URI "ldap://ldap.technikum-wien.at:389"
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE

using namespace std;
bool startLogin(ClientSocket* c, int trys)
{
   LDAP *ld;			/* LDAP resource handle */

   BerValue *servercredp;
   int rc=0;

   int ldapversion = LDAP_VERSION3;

   /* setup LDAP connection */
   if (ldap_initialize(&ld,LDAP_URI) != LDAP_SUCCESS)
   {
      fprintf(stderr,"ldap_init failed");
      return EXIT_FAILURE;
   }

   printf("connected to LDAP server %s\n",LDAP_URI);

   if ((rc = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &ldapversion)) != LDAP_SUCCESS)
   {
      fprintf(stderr, "ldap_set_option(PROTOCOL_VERSION): %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ld, NULL, NULL);
      return EXIT_FAILURE;
   }

   if ((rc = ldap_start_tls_s(ld, NULL, NULL)) != LDAP_SUCCESS)
   {
      fprintf(stderr, "ldap_start_tls_s(): %s\n", ldap_err2string(rc));
      ldap_unbind_ext_s(ld, NULL, NULL);
      return EXIT_FAILURE;
   }

   /* anonymous bind */
   string user;
   string password;
   BerValue cred;
   int i = 0;
   while(i < trys){
      string message;
      
      int size = c->recieveMessageWait(message);

      if( size <= 0)
      {
         printf("Client closed remote socket\n");
         return "";
      }
      user = cutOffTillStr(&message,"\n");
      user = "uid="+user+",ou=People,dc=technikum-wien,dc=at";
      cout << user << " lol \n";

      //cout << "Passwort(ACHTUNG PASSWORT WIRD ANGEZEIGT): ";
      password = message;

      cred.bv_val = (char *)password.c_str();
      cred.bv_len=strlen(password.c_str());

      rc = ldap_sasl_bind_s(ld,user.c_str(),LDAP_SASL_SIMPLE,&cred,NULL,NULL,&servercredp);
      if(rc == LDAP_SUCCESS)
      {
         c->sendMessage("Success");
         ldap_unbind_ext_s(ld, NULL, NULL);
         return true;
      }
      i++;
      if(i == trys)
         c->sendMessage("NO SOUP FOR YOU !!");
      else
         c->sendMessage("Failure");
   }
   ldap_unbind_ext_s(ld, NULL, NULL);
   return false;
}