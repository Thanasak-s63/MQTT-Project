//MQTT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "sub_pic.c"

#define ADDRESS     "203.146.252.179:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "kjpie"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int is_connected;
char sub_text[25] ;
char topic_name[50] = "kjpie" ;
char server_name[50] = "203.146.252.179:1883" ;
char *top_en ;
char *ser_en ;

volatile MQTTClient_deliveryToken deliveredtoken;
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{

    int i;
    char *payloadptr;
    payloadptr = message->payload;

    for(i=0; i < 24; i++)
    {
        sub_text[i] = payloadptr[i] ;
    }
    sub_text[24] = '\0' ;

    FILE *fp;
    fp = fopen("all_data.txt","a");
    fprintf(fp,"%s\n",sub_text);
    fclose(fp);

    sub_pic(sub_text);
    

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void to_connect_mqtt (MQTTClient *client ,MQTTClient_connectOptions conn_opts) {
    
    int ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((is_connected = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", is_connected);
        exit(EXIT_FAILURE);
    }

    MQTTClient_subscribe(client, TOPIC, QOS);

}

void to_connect_topic(MQTTClient client,MQTTClient_connectOptions conn_opts ) {

    MQTTClient_unsubscribe(client,TOPIC);
    
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((is_connected = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", is_connected);
        exit(EXIT_FAILURE);
    }

    MQTTClient_subscribe(client, topic_name, QOS);
    
}

void to_connect_server(MQTTClient client,MQTTClient_connectOptions conn_opts ) {

    MQTTClient_disconnect(client,10000);
    
    MQTTClient_create(&client, "localhost:1883", CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    if ((is_connected = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", is_connected);
        exit(EXIT_FAILURE);
    }

    MQTTClient_subscribe(client, topic_name, QOS);
}


//CURL
#include <stdio.h>
#include <curl/curl.h>

int sub_pic(char sub_text[100])
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char text[100] ;
    char *url = "sftp://admin:U%25%25FSM74Y%24GZ@203.146.252.179:22/~/web/sv1.kjpie/public_html/uploads/";
    char outfilename[100];
    curl = curl_easy_init();
    sprintf(text,"%s%s.jpg",url,sub_text);
    sprintf(outfilename,"sub_img/%s.jpg",sub_text);

    if (curl)
    {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, text);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if(res == CURLE_OK)
            printf("Download Successful\n");
        else
            printf("ERROR: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        fclose(fp);

        FILE *fpd;
        fpd = fopen("data.txt","a");
        fprintf(fpd,"%s\n",sub_text);
        fclose(fpd);
    }
    return 0;
}
