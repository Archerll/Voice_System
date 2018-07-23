/**
 *
 *author archer
*/
#include <stdio.h>
#include <string>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <sstream>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <exception>

using namespace std;


int flag=0;
string result;

int writer(char *data,size_t size,size_t nmemb,string *writerData)
{
    if(writerData == NULL)
    {
        return -1;
    }
    int len=size*nmemb;
    writerData->append(data,len);

    return len;
}

int parseJsonResonse(string input)
{
    Json::Value root;
    Json::Reader reader;
    cout<<"tuling server response json str:"<<input<<endl;
    bool parsingSuccessful = reader.parse(input , root);

    if(!parsingSuccessful)
    {
        std::cout<<"!!!Failed to parse the response dara"<<std::endl;
        return 1;
    }
    const Json::Value code= root["intent"]["code"];
    const Json::Value text= root["results"][0]["values"]["text"];
    result = text.asString();
    flag=1;
    std::cout<<"response code:"<<code<<std::endl;
    std::cout<<"response text:"<<text<<std::endl;

    return 0;
}

int HttpPostRequest(string input)
{
    string buffer;

    std::string strJson="{\"perception\": {\"inputText\": {";
    strJson +="\"text\": ";
    strJson +="\"";
    strJson += input;
    strJson +="\"";
    strJson +="}},";
    strJson +="\"userInfo\": {";
    strJson +="\"apiKey\": \"07af12c223604339a7ade76a31f10071\",";
    strJson +="\"userId\": \"123456\"}}";
    std::cout<<"post Json string:"<<strJson <<std::endl;
    try
    {
        CURL *pCurl=NULL;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);

        pCurl =curl_easy_init();
        if(NULL !=  pCurl)
        {
            curl_easy_setopt(pCurl,CURLOPT_TIMEOUT,5);

            curl_easy_setopt(pCurl,CURLOPT_URL,"http://openapi.tuling123.com/openapi/api/v2");

            curl_slist *plist =curl_slist_append(NULL,"Content-type:application/json;charset=UTF-8");
            curl_easy_setopt(pCurl,CURLOPT_HTTPHEADER,plist);

            curl_easy_setopt(pCurl,CURLOPT_POSTFIELDS,strJson.c_str());

            curl_easy_setopt(pCurl,CURLOPT_WRITEFUNCTION,writer);
            curl_easy_setopt(pCurl,CURLOPT_WRITEDATA,&buffer);

            res=curl_easy_perform(pCurl);

            if(res != CURLE_OK)
            {
                printf("curl_easy_perform{} failed:%s \n",curl_easy_strerror(res));    
            }
            curl_easy_cleanup(pCurl);
       }
       curl_global_cleanup();
    }
    catch(std::exception &ex)
    {
        printf("!!! curl exception %s.\n",ex.what());
    }
    if(buffer.empty())
    {
        cout<<"!!!error the tuling server response null"<<endl;
    }
    else
    {
        parseJsonResonse(buffer);
    }
    return 0;
}
/**
* get input,will auto send http post request to tuling server
*/
void nluCallBack(const std_msgs::String::ConstPtr& msg)
{
    std::cout <<"your question is:"<< msg->data << std::endl;
    HttpPostRequest(msg->data);
}
/**
 *main function
 */
int main(int argc,char **argv)
{
    ros::init(argc,argv,"tuling_nlu_node");
    ros::NodeHandle ndHandle;

    ros::Subscriber sub = ndHandle.subscribe("/voice/tuling_nlu_topic",10,nluCallBack);
    ros::Publisher pub=ndHandle.advertise<std_msgs::String>("/voice/xf_tts_topic",10);
    ros::Rate loop_rate(10);

    while (ros::ok())
    {
        if(flag)
        {
            std_msgs::String msg;
            msg.data = result;
            pub.publish(msg);
            flag = 0;
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
    
}
