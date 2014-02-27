/*
 *  Copyright (C) 2011 Justin Stoecker
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef RVDRAW_H
#define RVDRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <math.h>
#include "vector.h"
using namespace std;
using namespace salt;

#define ROBOVIS_HOST "127.0.0.1"
#define ROBOVIS_PORT "32769"
#define TEST_DURATION 10000
enum Color{
    GREEN,
    BRIGHT_BLUE,
    BLUE,
    RED,
    YELLO,
    BLACK,
    GRAY,
    PINK,
    VIOLET,
    ORANGE,
    WHITE,
    BROWN
};
class RVDraw{
private:
    struct addrinfo hints, *servinfo;
    int rv;
    int numbytes;
    int sockfd;
    struct addrinfo* p;
    double angle;

    inline int writeCharToBuf(unsigned char* buf, unsigned char value) {
      *buf = value;
      return 1;
    }

    inline int writeFloatToBuf(unsigned char* buf, float value) {
      char temp[20];
      sprintf(temp, "%6f", value);
      memcpy(buf, temp, 6);
      return 6;
    }

    inline int writeColorToBuf(unsigned char* buf, const float* color, int channels) {
      int i;
      for (i = 0; i < channels; i++)
        writeCharToBuf(buf+i, (unsigned char)(color[i]*255));
      return i;
    }

    inline int writeStringToBuf(unsigned char* buf, const string* text) {
      long i = 0;
      if (text != NULL)
        i += text->copy((char*)buf+i, text->length(), 0);
      i += writeCharToBuf(buf+i, 0);
      return i;
    }

    unsigned char* newBufferSwap(const string* name, int* bufSize) {
      *bufSize = 3 + ((name != NULL) ? name->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 0);
      i += writeCharToBuf(buf+i, 0);
      i += writeStringToBuf(buf+i, name);

      return buf;
    }

    unsigned char* newCircle(const float* center, float radius, float thickness,
        const float* color, const string* setName, int* bufSize) {

      *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, 0);
      i += writeFloatToBuf(buf+i, center[0]);
      i += writeFloatToBuf(buf+i, center[1]);
      i += writeFloatToBuf(buf+i, radius);
      i += writeFloatToBuf(buf+i, thickness);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newLine(const float* a, const float* b, float thickness,
        const float* color, const string* setName, int* bufSize) {

      *bufSize = 48 + ((setName != NULL) ? setName->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, 1);
      i += writeFloatToBuf(buf+i, a[0]);
      i += writeFloatToBuf(buf+i, a[1]);
      i += writeFloatToBuf(buf+i, a[2]);
      i += writeFloatToBuf(buf+i, b[0]);
      i += writeFloatToBuf(buf+i, b[1]);
      i += writeFloatToBuf(buf+i, b[2]);
      i += writeFloatToBuf(buf+i, thickness);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newPoint(const float* p, float size, const float* color,
        const string* setName, int* bufSize) {

      *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, 2);
      i += writeFloatToBuf(buf+i, p[0]);
      i += writeFloatToBuf(buf+i, p[1]);
      i += writeFloatToBuf(buf+i, p[2]);
      i += writeFloatToBuf(buf+i, size);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newSphere(const float* p, float radius, const float* color,
        const string* setName, int* bufSize) {

      *bufSize = 30 + ((setName != NULL) ? setName->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, 3);
      i += writeFloatToBuf(buf+i, p[0]);
      i += writeFloatToBuf(buf+i, p[1]);
      i += writeFloatToBuf(buf+i, p[2]);
      i += writeFloatToBuf(buf+i, radius);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newPolygon(const float* v, int numVerts, const float* color,
        const string* setName, int* bufSize) {

      *bufSize = 18 * numVerts + 8 + ((setName != NULL) ? setName->length() : 0);
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 1);
      i += writeCharToBuf(buf+i, 4);
      i += writeCharToBuf(buf+i, numVerts);
      i += writeColorToBuf(buf+i, color, 4);

      for (int j = 0; j < numVerts; j++) {
        i += writeFloatToBuf(buf+i, v[j*3+0]);
        i += writeFloatToBuf(buf+i, v[j*3+1]);
        i += writeFloatToBuf(buf+i, v[j*3+2]);
      }

      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newAnnotation(const string* text, const float* p,
        const float* color, const string* setName, int* bufSize) {

      *bufSize = 25 + text->length() + setName->length();
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 2);
      i += writeCharToBuf(buf+i, 0);
      i += writeFloatToBuf(buf+i, p[0]);
      i += writeFloatToBuf(buf+i, p[1]);
      i += writeFloatToBuf(buf+i, p[2]);
      i += writeColorToBuf(buf+i, color, 3);
      i += writeStringToBuf(buf+i, text);
      i += writeStringToBuf(buf+i, setName);

      return buf;
    }

    unsigned char* newAgentAnnotation(const string* text, bool leftTeam,
        int agentNum, const float* color, int* bufSize) {

      *bufSize = (text == NULL) ? 3 : 7 + text->length();
      unsigned char* buf = new unsigned char[*bufSize];

      long i = 0;
      i += writeCharToBuf(buf+i, 2);

      if (text == NULL) {
        i += writeCharToBuf(buf+i, 2);
        i += writeCharToBuf(buf+i, (leftTeam ? agentNum - 1 : agentNum + 127));
      } else {
        i += writeCharToBuf(buf+i, 1);
        i += writeCharToBuf(buf+i, (leftTeam ? agentNum - 1 : agentNum + 127));
        i += writeColorToBuf(buf+i, color, 3);
        i += writeStringToBuf(buf+i, text);
      }

      return buf;
    }

    RVDraw(){
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        if ((rv = getaddrinfo(ROBOVIS_HOST, ROBOVIS_PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        }

        // loop through all the results and make a socket
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("socket");
                continue;
            }

            break;
        }

        if (p == NULL) {
            fprintf(stderr, "failed to bind socket\n");
        }
        angle = 0;
    }

    void swapBuffers(const string* setName) {
      int bufSize = -1;
      unsigned char* buf = newBufferSwap(setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float thickness, float r, float g, float b,
        const string* setName) {
      float pa[3] = {x1,y1,z1};
      float pb[3] = {x2,y2,z2};
      float color[3] = {r,g,b};

      int bufSize = -1;
      unsigned char* buf = newLine(pa, pb, thickness, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawCircle(float x, float y, float radius, float thickness, float r, float g, float b, const string* setName) {
      float center[2] = {x,y};
      float color[3] = {r,g,b};

      int bufSize = -1;
      unsigned char* buf = newCircle(center, radius, thickness, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawSphere(float x, float y, float z, float radius, float r, float g, float b, const string* setName) {
      float center[3] = {x,y,z};
      float color[3] = {r,g,b};

      int bufSize = -1;
      unsigned char* buf = newSphere(center, radius, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawPoint(float x, float y, float z, float size, float r, float g, float b, const string* setName) {
      float center[3] = {x,y,z};
      float color[3] = {r,g,b};

      int bufSize = -1;
      unsigned char* buf = newPoint(center, size, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawPolygon(const float* v, int numVerts, float r, float g, float b,
        float a, const string* setName) {
      float color[4] = {r,g,b,a};

      int bufSize = -1;
      unsigned char* buf = newPolygon(v, numVerts, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void drawAnnotation(const string* text, float x, float y, float z, float r,
        float g, float b, const string* setName) {
      float color[3] = {r,g,b};
      float pos[3] = {x,y,z};

      int bufSize = -1;
      unsigned char* buf = newAnnotation(text, pos, color, setName, &bufSize);
      sendto(sockfd, buf, bufSize, 0, p->ai_addr, p->ai_addrlen);
      delete[] buf;
    }

    void fillColor(Color c,float rgb[]){

        switch(c){
            case RED:
                rgb[0]=1;
                rgb[1]=0;
                rgb[2]=0;
            break;

            case GREEN:
                rgb[0]=0;
                rgb[1]=1;
                rgb[2]=0;
            break;

            case BLUE:
                rgb[0]=0;
                rgb[1]=0;
                rgb[2]=1;
            break;

            case WHITE:
                rgb[0]=1;
                rgb[1]=1;
                rgb[2]=1;
            break;

            case BLACK:
                rgb[0]=0;
                rgb[1]=0;
                rgb[2]=0;
            break;

            case YELLO:
                rgb[0]=1;
                rgb[1]=1;
                rgb[2]=0;
            break;

            case BRIGHT_BLUE:
                rgb[0]=0;
                rgb[1]=1;
                rgb[2]=1;
            break;

            case ORANGE:
                rgb[0]=1;
                rgb[1]=0.6;
                rgb[2]=0;
            break;

            case PINK:
                rgb[0]=1;
                rgb[1]=0;
                rgb[2]=0.5;
            break;

            case VIOLET:
                rgb[0]=1;
                rgb[1]=0;
                rgb[2]=1;
            break;

            case GRAY:
                rgb[0]=0.6;
                rgb[1]=0.6;
                rgb[2]=0.6;
            break;

            case BROWN:
                rgb[0]=0.6;
                rgb[1]=0.3;
                rgb[2]=0;
            break;
        }
    }

public:

    static RVDraw *instance()
    {
        static RVDraw *ins = new RVDraw();
        return ins;
    }

    ~RVDraw(){
        freeaddrinfo(servinfo);
        close(sockfd);
    }



    float maxf(float a, float b) {
      return a > b ? a : b;
    }

    void drawVector3f(Vector3f iPos, Color iCo, int iNumber, double iSize=8)
    {
        string vec("vector3f.");
        char buf[10];
        sprintf(buf, "%d", iNumber);
        vec += buf;
        float rgb[3];
        fillColor(iCo, rgb);
        drawPoint(float(iPos.x()), float(iPos.y()), float(iPos.z()), float(iSize), rgb[0], rgb[1], rgb[2], &vec);
        swapBuffers(&vec);
    }
    void drawLine(Vector3f iPos1, Vector3f iPos2, Color iCo, int iNumber, double iSize=2)
    {
        string lin("line.");
        char buf[10];
        sprintf(buf, "%d", iNumber);
        lin += buf;
        float rgb[3];
        fillColor(iCo, rgb);
        drawLine(float(iPos1.x()), float(iPos1.y()), float(iPos1.z()), float(iPos2.x()), float(iPos2.y()), float(iPos2.z()), iSize,rgb[0], rgb[1], rgb[2], &lin);
        swapBuffers(&lin);
    }
#include "Geom.h"

    void drawLine(VecPosition iPos1, VecPosition iPos2, Color iCo, int iNumber, double iSize=2)
    {
        drawLine(Vector3f(iPos1.getX(),iPos1.getY(),0),Vector3f(iPos2.getX(),iPos2.getY(),0),iCo,iNumber,iSize);
    }
    void drawCircle(Vector3f iPos, double iRadius, Color iCo, int iNumber, double iSize=1)
    {
        string circ("circle.");
        char buf[10];
        sprintf(buf, "%d", iNumber);
        circ += buf;
        float rgb[3];
        fillColor(iCo, rgb);
        drawCircle(float(iPos.x()), float(iPos.y()), float(iRadius), iSize, rgb[0], rgb[1], rgb[2], &circ);
        swapBuffers(&circ);
    }

    void drawSphere(Vector3f iPos, double iRadius, Color iCo, int iNumber)
    {
        string circ("sphere.");
            char buf[10];
            sprintf(buf, "%d", iNumber);
            circ += buf;
        float rgb[3];
        fillColor(iCo, rgb);
        drawSphere(float(iPos.x()), float(iPos.y()), float(iPos.z()), float(iRadius), rgb[0], rgb[1], rgb[2], &circ);
        swapBuffers(&circ);
    }




};


#endif
