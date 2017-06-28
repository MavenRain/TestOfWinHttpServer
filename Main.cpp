#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Http.h>
#include <cstdio>
#pragma comment(lib, "httpapi.lib")

void main()
{
    HTTPAPI_VERSION HttpApiVersion = {2, 0};
    HttpInitialize(HttpApiVersion, HTTP_INITIALIZE_SERVER, NULL);
    void * requestQueue;
    HttpCreateHttpHandle(& requestQueue, 0);
    HttpAddUrl(requestQueue, L"http://localhost:9090/", 0);
    HttpAddUrl(requestQueue, L"http://localhost:9090/jimmy", 0);
    HTTP_REQUEST_ID requestIdentifier = HTTP_NULL_ID;
    char request[sizeof(HTTP_REQUEST) + 4096] = {0};
    while(true)
    {
        HttpReceiveHttpRequest(requestQueue, requestIdentifier, 0, (PHTTP_REQUEST) request, sizeof(HTTP_REQUEST) + 4096, NULL, NULL);
        switch(((PHTTP_REQUEST)request)->Verb)
        {
        case HttpVerbGET:
            printf("Got a GET request\r\n");
            HTTP_RESPONSE response;
            RtlZeroMemory(& response, sizeof(response));
            response.StatusCode = 200;
            response.pReason = "OK";
            response.ReasonLength = sizeof(response.pReason);
            response.Headers.KnownHeaders[HttpHeaderContentType].pRawValue = "text/html";
            response.Headers.KnownHeaders[HttpHeaderContentType].RawValueLength = sizeof(response.Headers.KnownHeaders[HttpHeaderContentType].pRawValue);
            HttpSendHttpResponse(requestQueue, ((PHTTP_REQUEST)request)->RequestId, 0, & response, NULL, NULL, NULL, 0, NULL, NULL);
            break;
        default:
            printf("Invalid command\r\n");
            break;
        }
    }
}