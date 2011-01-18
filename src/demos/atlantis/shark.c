/* atlantis --- Shows moving 3D sea animals */

#if 0
static const char sccsid[] = "@(#)shark.c	1.2 98/06/16 xlockmore";
#endif

/* Copyright (c) E. Lassauge, 1998. */

/*
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
 *
 * The original code for this mode was written by Mark J. Kilgard
 * as a demo for openGL programming.
 * 
 * Porting it to xlock  was possible by comparing the original Mesa's morph3d 
 * demo with it's ported version to xlock, so thanks for Marcelo F. Vianna 
 * (look at morph3d.c) for his indirect help.
 *
 * Thanks goes also to Brian Paul for making it possible and inexpensive
 * to use OpenGL at home.
 *
 * My e-mail address is lassauge@users.sourceforge.net
 *
 * Eric Lassauge  (May-13-1998)
 *
 */

/**
 * (c) Copyright 1993, 1994, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

class shark:public fishRec
{
float N002[3];
float N003[3];
float N004[3];
float N005[3];
float N006[3];
float N007[3];
float N008[3];
float N009[3];
float N010[3];
float N011[3];
float N012[3];
float N013[3];
float N014[3];
float N015[3];
float N016[3];
float N017[3];
float N018[3];
float N019[3];
float N020[3];
float N021[3];
float N022[3];
float N023[3];
float N024[3];
float N025[3];
float N026[3];
float N027[3];
float N028[3];
float N029[3];
float N030[3];
float N031[3];
float N032[3];
float N033[3];
float N034[3];
float N035[3];
float N036[3];
float N037[3];
float N038[3];
float N039[3];
float N040[3];
float N041[3];
float N042[3];
float N043[3];
float N044[3];
float N045[3];
float N046[3];
float N047[3];
float N048[3];
float N049[3];
float N050[3];
float N051[3];
float N052[3];
float N058[3];
float N059[3];
float N060[3];
float N061[3];
float N062[3];
float N063[3];
float N064[3];
float N065[3];
float N066[3];
float N069[3];
float N070[3];
float P003[3];
float P005[3];
float P006[3];
float P008[3];
float P009[3];
float P012[3];
float P013[3];
float P014[3];
float P015[3];
float P016[3];
float P017[3];
float P018[3];
float P019[3];
float P020[3];
float P021[3];
float P022[3];
float P058[3];
float P059[3];
float P060[3];
float P062[3];
float P063[3];
float P064[3];
float P065[3];
float P066[3];
float P071[3];
float P072[3];
float P073[3];
float P074[3];
float P075[3];
float P076[3];
float P077[3];
float P078[3];
float P079[3];
float P080[3];
float P081[3];
float P082[3];
float iP002[3];
float iP004[3];
float iP007[3];
float iP010[3];
float iP011[3];
float iP023[3];
float iP024[3];
float iP025[3];
float iP026[3];
float iP027[3];
float iP028[3];
float iP029[3];
float iP030[3];
float iP031[3];
float iP032[3];
float iP033[3];
float iP034[3];
float iP035[3];
float iP036[3];
float iP037[3];
float iP038[3];
float iP039[3];
float iP040[3];
float iP041[3];
float iP042[3];
float iP043[3];
float iP044[3];
float iP045[3];
float iP046[3];
float iP047[3];
float iP048[3];
float iP049[3];
float iP050[3];
float iP051[3];
float iP052[3];
float iP061[3];
float iP069[3];
float iP070[3];




       float P002[3];
       float P004[3];
       float P007[3];
       float P010[3];
       float P011[3];
       float P023[3];
       float P024[3];
       float P025[3];
       float P026[3];
       float P027[3];
       float P028[3];
       float P029[3];
       float P030[3];
       float P031[3];
       float P032[3];
       float P033[3];
       float P034[3];
       float P035[3];
       float P036[3];
       float P037[3];
       float P038[3];
       float P039[3];
       float P040[3];
       float P041[3];
       float P042[3];
       float P043[3];
       float P044[3];
       float P045[3];
       float P046[3];
       float P047[3];
       float P048[3];
       float P049[3];
       float P050[3];
       float P051[3];
       float P052[3];
       float P061[3];
       float P069[3];
       float P070[3];












 void
Fish001(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N005);
	glVertex3fv(P005);
	glNormal3fv(N059);
	glVertex3fv(P059);
	glNormal3fv(N060);
	glVertex3fv(P060);
	glNormal3fv(N006);
	glVertex3fv(P006);
	glEnd();
	glBegin(cap);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N005);
	glVertex3fv(P005);
	glNormal3fv(N006);
	glVertex3fv(P006);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glEnd();
	glBegin(cap);
	glNormal3fv(N006);
	glVertex3fv(P006);
	glNormal3fv(N060);
	glVertex3fv(P060);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glEnd();
	glBegin(cap);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glNormal3fv(N006);
	glVertex3fv(P006);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glEnd();
	glBegin(cap);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glNormal3fv(N017);
	glVertex3fv(P017);
	glEnd();
	glBegin(cap);
	glNormal3fv(N017);
	glVertex3fv(P017);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glEnd();
	glBegin(cap);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glNormal3fv(N009);
	glVertex3fv(P009);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glEnd();
	glBegin(cap);
	glNormal3fv(N008);
	glVertex3fv(P008);
	glNormal3fv(N060);
	glVertex3fv(P060);
	glNormal3fv(N009);
	glVertex3fv(P009);
	glEnd();
	glBegin(cap);
	glNormal3fv(N007);
	glVertex3fv(P007);
	glNormal3fv(N010);
	glVertex3fv(P010);
	glNormal3fv(N009);
	glVertex3fv(P009);
	glEnd();
	glBegin(cap);
	glNormal3fv(N009);
	glVertex3fv(P009);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glEnd();
	glBegin(cap);
	glNormal3fv(N009);
	glVertex3fv(P009);
	glNormal3fv(N010);
	glVertex3fv(P010);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glEnd();
	glBegin(cap);
	glNormal3fv(N010);
	glVertex3fv(P010);
	glNormal3fv(N020);
	glVertex3fv(P020);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glEnd();
	glBegin(cap);
	glNormal3fv(N010);
	glVertex3fv(P010);
	glNormal3fv(N011);
	glVertex3fv(P011);
	glNormal3fv(N021);
	glVertex3fv(P021);
	glNormal3fv(N020);
	glVertex3fv(P020);
	glEnd();
	glBegin(cap);
	glNormal3fv(N004);
	glVertex3fv(P004);
	glNormal3fv(N011);
	glVertex3fv(P011);
	glNormal3fv(N010);
	glVertex3fv(P010);
	glNormal3fv(N007);
	glVertex3fv(P007);
	glEnd();
	glBegin(cap);
	glNormal3fv(N004);
	glVertex3fv(P004);
	glNormal3fv(N012);
	glVertex3fv(P012);
	glNormal3fv(N011);
	glVertex3fv(P011);
	glEnd();
	glBegin(cap);
	glNormal3fv(N012);
	glVertex3fv(P012);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N011);
	glVertex3fv(P011);
	glEnd();
	glBegin(cap);
	glNormal3fv(N011);
	glVertex3fv(P011);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N021);
	glVertex3fv(P021);
	glEnd();
	glBegin(cap);
	glNormal3fv(N059);
	glVertex3fv(P059);
	glNormal3fv(N005);
	glVertex3fv(P005);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glEnd();
	glBegin(cap);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N014);
	glVertex3fv(P014);
	glNormal3fv(N003);
	glVertex3fv(P003);
	glEnd();
	glBegin(cap);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N003);
	glVertex3fv(P003);
	glNormal3fv(N059);
	glVertex3fv(P059);
	glEnd();
	glBegin(cap);
	glNormal3fv(N014);
	glVertex3fv(P014);
	glNormal3fv(N013);
	glVertex3fv(P013);
	glNormal3fv(N003);
	glVertex3fv(P003);
	glEnd();
	glBegin(cap);
	glNormal3fv(N003);
	glVertex3fv(P003);
	glNormal3fv(N012);
	glVertex3fv(P012);
	glNormal3fv(N059);
	glVertex3fv(P059);
	glEnd();
	glBegin(cap);
	glNormal3fv(N013);
	glVertex3fv(P013);
	glNormal3fv(N012);
	glVertex3fv(P012);
	glNormal3fv(N003);
	glVertex3fv(P003);
	glEnd();
	glBegin(cap);
	glNormal3fv(N013);
	glVertex3fv(P013);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N012);
	glVertex3fv(P012);
	glEnd();
	glBegin(cap);
	glVertex3fv(P071);
	glVertex3fv(P072);
	glVertex3fv(P073);
	glVertex3fv(P074);
	glVertex3fv(P075);
	glVertex3fv(P076);
	glEnd();
	glBegin(cap);
	glVertex3fv(P077);
	glVertex3fv(P078);
	glVertex3fv(P079);
	glVertex3fv(P080);
	glVertex3fv(P081);
	glVertex3fv(P082);
	glEnd();
}

 void
Fish002(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N013);
	glVertex3fv(P013);
	glNormal3fv(N014);
	glVertex3fv(P014);
	glNormal3fv(N024);
	glVertex3fv(P024);
	glNormal3fv(N023);
	glVertex3fv(P023);
	glEnd();
	glBegin(cap);
	glNormal3fv(N014);
	glVertex3fv(P014);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N025);
	glVertex3fv(P025);
	glNormal3fv(N024);
	glVertex3fv(P024);
	glEnd();
	glBegin(cap);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glNormal3fv(N017);
	glVertex3fv(P017);
	glNormal3fv(N027);
	glVertex3fv(P027);
	glNormal3fv(N026);
	glVertex3fv(P026);
	glEnd();
	glBegin(cap);
	glNormal3fv(N017);
	glVertex3fv(P017);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glNormal3fv(N028);
	glVertex3fv(P028);
	glNormal3fv(N027);
	glVertex3fv(P027);
	glEnd();
	glBegin(cap);
	glNormal3fv(N020);
	glVertex3fv(P020);
	glNormal3fv(N021);
	glVertex3fv(P021);
	glNormal3fv(N031);
	glVertex3fv(P031);
	glNormal3fv(N030);
	glVertex3fv(P030);
	glEnd();
	glBegin(cap);
	glNormal3fv(N013);
	glVertex3fv(P013);
	glNormal3fv(N023);
	glVertex3fv(P023);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glEnd();
	glBegin(cap);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N023);
	glVertex3fv(P023);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glEnd();
	glBegin(cap);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glNormal3fv(N031);
	glVertex3fv(P031);
	glEnd();
	glBegin(cap);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N031);
	glVertex3fv(P031);
	glNormal3fv(N021);
	glVertex3fv(P021);
	glEnd();
	glBegin(cap);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glEnd();
	glBegin(cap);
	glNormal3fv(N018);
	glVertex3fv(P018);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glNormal3fv(N028);
	glVertex3fv(P028);
	glEnd();
	glBegin(cap);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N020);
	glVertex3fv(P020);
	glNormal3fv(N030);
	glVertex3fv(P030);
	glEnd();
	glBegin(cap);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N030);
	glVertex3fv(P030);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glEnd();
}

 void
Fish003(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glNormal3fv(N023);
	glVertex3fv(P023);
	glNormal3fv(N033);
	glVertex3fv(P033);
	glNormal3fv(N042);
	glVertex3fv(P042);
	glEnd();
	glBegin(cap);
	glNormal3fv(N031);
	glVertex3fv(P031);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glNormal3fv(N042);
	glVertex3fv(P042);
	glNormal3fv(N041);
	glVertex3fv(P041);
	glEnd();
	glBegin(cap);
	glNormal3fv(N023);
	glVertex3fv(P023);
	glNormal3fv(N024);
	glVertex3fv(P024);
	glNormal3fv(N034);
	glVertex3fv(P034);
	glNormal3fv(N033);
	glVertex3fv(P033);
	glEnd();
	glBegin(cap);
	glNormal3fv(N024);
	glVertex3fv(P024);
	glNormal3fv(N025);
	glVertex3fv(P025);
	glNormal3fv(N035);
	glVertex3fv(P035);
	glNormal3fv(N034);
	glVertex3fv(P034);
	glEnd();
	glBegin(cap);
	glNormal3fv(N030);
	glVertex3fv(P030);
	glNormal3fv(N031);
	glVertex3fv(P031);
	glNormal3fv(N041);
	glVertex3fv(P041);
	glNormal3fv(N040);
	glVertex3fv(P040);
	glEnd();
	glBegin(cap);
	glNormal3fv(N025);
	glVertex3fv(P025);
	glNormal3fv(N026);
	glVertex3fv(P026);
	glNormal3fv(N036);
	glVertex3fv(P036);
	glNormal3fv(N035);
	glVertex3fv(P035);
	glEnd();
	glBegin(cap);
	glNormal3fv(N026);
	glVertex3fv(P026);
	glNormal3fv(N027);
	glVertex3fv(P027);
	glNormal3fv(N037);
	glVertex3fv(P037);
	glNormal3fv(N036);
	glVertex3fv(P036);
	glEnd();
	glBegin(cap);
	glNormal3fv(N027);
	glVertex3fv(P027);
	glNormal3fv(N028);
	glVertex3fv(P028);
	glNormal3fv(N038);
	glVertex3fv(P038);
	glNormal3fv(N037);
	glVertex3fv(P037);
	glEnd();
	glBegin(cap);
	glNormal3fv(N028);
	glVertex3fv(P028);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glNormal3fv(N039);
	glVertex3fv(P039);
	glNormal3fv(N038);
	glVertex3fv(P038);
	glEnd();
	glBegin(cap);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glNormal3fv(N030);
	glVertex3fv(P030);
	glNormal3fv(N040);
	glVertex3fv(P040);
	glNormal3fv(N039);
	glVertex3fv(P039);
	glEnd();
}

 void
Fish004(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N040);
	glVertex3fv(P040);
	glNormal3fv(N041);
	glVertex3fv(P041);
	glNormal3fv(N051);
	glVertex3fv(P051);
	glNormal3fv(N050);
	glVertex3fv(P050);
	glEnd();
	glBegin(cap);
	glNormal3fv(N041);
	glVertex3fv(P041);
	glNormal3fv(N042);
	glVertex3fv(P042);
	glNormal3fv(N052);
	glVertex3fv(P052);
	glNormal3fv(N051);
	glVertex3fv(P051);
	glEnd();
	glBegin(cap);
	glNormal3fv(N042);
	glVertex3fv(P042);
	glNormal3fv(N033);
	glVertex3fv(P033);
	glNormal3fv(N043);
	glVertex3fv(P043);
	glNormal3fv(N052);
	glVertex3fv(P052);
	glEnd();
	glBegin(cap);
	glNormal3fv(N033);
	glVertex3fv(P033);
	glNormal3fv(N034);
	glVertex3fv(P034);
	glNormal3fv(N044);
	glVertex3fv(P044);
	glNormal3fv(N043);
	glVertex3fv(P043);
	glEnd();
	glBegin(cap);
	glNormal3fv(N034);
	glVertex3fv(P034);
	glNormal3fv(N035);
	glVertex3fv(P035);
	glNormal3fv(N045);
	glVertex3fv(P045);
	glNormal3fv(N044);
	glVertex3fv(P044);
	glEnd();
	glBegin(cap);
	glNormal3fv(N035);
	glVertex3fv(P035);
	glNormal3fv(N036);
	glVertex3fv(P036);
	glNormal3fv(N046);
	glVertex3fv(P046);
	glNormal3fv(N045);
	glVertex3fv(P045);
	glEnd();
	glBegin(cap);
	glNormal3fv(N036);
	glVertex3fv(P036);
	glNormal3fv(N037);
	glVertex3fv(P037);
	glNormal3fv(N047);
	glVertex3fv(P047);
	glNormal3fv(N046);
	glVertex3fv(P046);
	glEnd();
	glBegin(cap);
	glNormal3fv(N037);
	glVertex3fv(P037);
	glNormal3fv(N038);
	glVertex3fv(P038);
	glNormal3fv(N048);
	glVertex3fv(P048);
	glNormal3fv(N047);
	glVertex3fv(P047);
	glEnd();
	glBegin(cap);
	glNormal3fv(N038);
	glVertex3fv(P038);
	glNormal3fv(N039);
	glVertex3fv(P039);
	glNormal3fv(N049);
	glVertex3fv(P049);
	glNormal3fv(N048);
	glVertex3fv(P048);
	glEnd();
	glBegin(cap);
	glNormal3fv(N039);
	glVertex3fv(P039);
	glNormal3fv(N040);
	glVertex3fv(P040);
	glNormal3fv(N050);
	glVertex3fv(P050);
	glNormal3fv(N049);
	glVertex3fv(P049);
	glEnd();
	glBegin(cap);
	glNormal3fv(N070);
	glVertex3fv(P070);
	glNormal3fv(N061);
	glVertex3fv(P061);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glEnd();
	glBegin(cap);
	glNormal3fv(N061);
	glVertex3fv(P061);
	glNormal3fv(N046);
	glVertex3fv(P046);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glEnd();
	glBegin(cap);
	glNormal3fv(N045);
	glVertex3fv(P045);
	glNormal3fv(N046);
	glVertex3fv(P046);
	glNormal3fv(N061);
	glVertex3fv(P061);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N061);
	glVertex3fv(P061);
	glNormal3fv(N070);
	glVertex3fv(P070);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N045);
	glVertex3fv(P045);
	glNormal3fv(N061);
	glVertex3fv(P061);
	glEnd();
}

 void
Fish005(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N044);
	glVertex3fv(P044);
	glNormal3fv(N045);
	glVertex3fv(P045);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N043);
	glVertex3fv(P043);
	glNormal3fv(N044);
	glVertex3fv(P044);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N052);
	glVertex3fv(P052);
	glNormal3fv(N043);
	glVertex3fv(P043);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N051);
	glVertex3fv(P051);
	glNormal3fv(N052);
	glVertex3fv(P052);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N046);
	glVertex3fv(P046);
	glNormal3fv(N047);
	glVertex3fv(P047);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N047);
	glVertex3fv(P047);
	glNormal3fv(N048);
	glVertex3fv(P048);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N048);
	glVertex3fv(P048);
	glNormal3fv(N049);
	glVertex3fv(P049);
	glEnd();
	glBegin(cap);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N049);
	glVertex3fv(P049);
	glNormal3fv(N050);
	glVertex3fv(P050);
	glEnd();
	glBegin(cap);
	glNormal3fv(N050);
	glVertex3fv(P050);
	glNormal3fv(N051);
	glVertex3fv(P051);
	glNormal3fv(N069);
	glVertex3fv(P069);
	glEnd();
	glBegin(cap);
	glNormal3fv(N051);
	glVertex3fv(P051);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glNormal3fv(N069);
	glVertex3fv(P069);
	glEnd();
	glBegin(cap);
	glNormal3fv(N050);
	glVertex3fv(P050);
	glNormal3fv(N069);
	glVertex3fv(P069);
	glNormal3fv(N002);
	glVertex3fv(P002);
	glEnd();
}

 void
Fish006(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N066);
	glVertex3fv(P066);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glNormal3fv(N026);
	glVertex3fv(P026);
	glEnd();
	glBegin(cap);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N066);
	glVertex3fv(P066);
	glNormal3fv(N025);
	glVertex3fv(P025);
	glEnd();
	glBegin(cap);
	glNormal3fv(N025);
	glVertex3fv(P025);
	glNormal3fv(N066);
	glVertex3fv(P066);
	glNormal3fv(N026);
	glVertex3fv(P026);
	glEnd();
	glBegin(cap);
	glNormal3fv(N066);
	glVertex3fv(P066);
	glNormal3fv(N058);
	glVertex3fv(P058);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glEnd();
	glBegin(cap);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N058);
	glVertex3fv(P058);
	glNormal3fv(N066);
	glVertex3fv(P066);
	glEnd();
	glBegin(cap);
	glNormal3fv(N058);
	glVertex3fv(P058);
	glNormal3fv(N015);
	glVertex3fv(P015);
	glNormal3fv(N016);
	glVertex3fv(P016);
	glEnd();
}

 void
Fish007(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N062);
	glVertex3fv(P062);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glEnd();
	glBegin(cap);
	glNormal3fv(N062);
	glVertex3fv(P062);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glNormal3fv(N064);
	glVertex3fv(P064);
	glEnd();
	glBegin(cap);
	glNormal3fv(N022);
	glVertex3fv(P022);
	glNormal3fv(N062);
	glVertex3fv(P062);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glEnd();
	glBegin(cap);
	glNormal3fv(N062);
	glVertex3fv(P062);
	glNormal3fv(N064);
	glVertex3fv(P064);
	glNormal3fv(N032);
	glVertex3fv(P032);
	glEnd();
}

 void
Fish008(GLenum cap)
{
	glBegin(cap);
	glNormal3fv(N063);
	glVertex3fv(P063);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glEnd();
	glBegin(cap);
	glNormal3fv(N019);
	glVertex3fv(P019);
	glNormal3fv(N063);
	glVertex3fv(P063);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glEnd();
	glBegin(cap);
	glNormal3fv(N063);
	glVertex3fv(P063);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glNormal3fv(N065);
	glVertex3fv(P065);
	glEnd();
	glBegin(cap);
	glNormal3fv(N063);
	glVertex3fv(P063);
	glNormal3fv(N065);
	glVertex3fv(P065);
	glNormal3fv(N029);
	glVertex3fv(P029);
	glEnd();
}

 void
Fish009(GLenum cap)
{
	glBegin(cap);
	glVertex3fv(P059);
	glVertex3fv(P012);
	glVertex3fv(P009);
	glVertex3fv(P060);
	glEnd();
	glBegin(cap);
	glVertex3fv(P012);
	glVertex3fv(P004);
	glVertex3fv(P007);
	glVertex3fv(P009);
	glEnd();
}

 void
Fish_1(GLenum cap)
{
	Fish004(cap);
	Fish005(cap);
	Fish003(cap);
	Fish007(cap);
	Fish006(cap);
	Fish002(cap);
	Fish008(cap);
	Fish009(cap);
	Fish001(cap);
}

 void
Fish_2(GLenum cap)
{
	Fish005(cap);
	Fish004(cap);
	Fish003(cap);
	Fish008(cap);
	Fish006(cap);
	Fish002(cap);
	Fish007(cap);
	Fish009(cap);
	Fish001(cap);
}

 void
Fish_3(GLenum cap)
{
	Fish005(cap);
	Fish004(cap);
	Fish007(cap);
	Fish003(cap);
	Fish002(cap);
	Fish008(cap);
	Fish009(cap);
	Fish001(cap);
	Fish006(cap);
}

 void
Fish_4(GLenum cap)
{
	Fish005(cap);
	Fish004(cap);
	Fish008(cap);
	Fish003(cap);
	Fish002(cap);
	Fish007(cap);
	Fish009(cap);
	Fish001(cap);
	Fish006(cap);
}

 void
Fish_5(GLenum cap)
{
	Fish009(cap);
	Fish006(cap);
	Fish007(cap);
	Fish001(cap);
	Fish002(cap);
	Fish003(cap);
	Fish008(cap);
	Fish004(cap);
	Fish005(cap);
}

 void
Fish_6(GLenum cap)
{
	Fish009(cap);
	Fish006(cap);
	Fish008(cap);
	Fish001(cap);
	Fish002(cap);
	Fish007(cap);
	Fish003(cap);
	Fish004(cap);
	Fish005(cap);
}

 void
Fish_7(GLenum cap)
{
	Fish009(cap);
	Fish001(cap);
	Fish007(cap);
	Fish005(cap);
	Fish002(cap);
	Fish008(cap);
	Fish003(cap);
	Fish004(cap);
	Fish006(cap);
}

 void
Fish_8(GLenum cap)
{
	Fish009(cap);
	Fish008(cap);
	Fish001(cap);
	Fish002(cap);
	Fish007(cap);
	Fish003(cap);
	Fish005(cap);
	Fish004(cap);
	Fish006(cap);
}
public:
void
Draw()
{
	float       mat[4][4];
	int         n;
	float       seg1, seg2, seg3, seg4, segup;
	float       thrash, chomp;
	GLenum      cap;

	htail = (int) (htail - (int) (5 * v)) % 360;

	thrash = 50 * v;

	seg1 = 0.6 * thrash * sin(htail * RRAD);
	seg2 = 1.8 * thrash * sin((htail + 45) * RRAD);
	seg3 = 3 * thrash * sin((htail + 90) * RRAD);
	seg4 = 4 * thrash * sin((htail + 110) * RRAD);

	chomp = 0;
	if (v > 2) {
		chomp = -(v - 2) * 200;
	}
	P004[1] = iP004[1] + chomp;
	P007[1] = iP007[1] + chomp;
	P010[1] = iP010[1] + chomp;
	P011[1] = iP011[1] + chomp;

	P023[0] = iP023[0] + seg1;
	P024[0] = iP024[0] + seg1;
	P025[0] = iP025[0] + seg1;
	P026[0] = iP026[0] + seg1;
	P027[0] = iP027[0] + seg1;
	P028[0] = iP028[0] + seg1;
	P029[0] = iP029[0] + seg1;
	P030[0] = iP030[0] + seg1;
	P031[0] = iP031[0] + seg1;
	P032[0] = iP032[0] + seg1;
	P033[0] = iP033[0] + seg2;
	P034[0] = iP034[0] + seg2;
	P035[0] = iP035[0] + seg2;
	P036[0] = iP036[0] + seg2;
	P037[0] = iP037[0] + seg2;
	P038[0] = iP038[0] + seg2;
	P039[0] = iP039[0] + seg2;
	P040[0] = iP040[0] + seg2;
	P041[0] = iP041[0] + seg2;
	P042[0] = iP042[0] + seg2;
	P043[0] = iP043[0] + seg3;
	P044[0] = iP044[0] + seg3;
	P045[0] = iP045[0] + seg3;
	P046[0] = iP046[0] + seg3;
	P047[0] = iP047[0] + seg3;
	P048[0] = iP048[0] + seg3;
	P049[0] = iP049[0] + seg3;
	P050[0] = iP050[0] + seg3;
	P051[0] = iP051[0] + seg3;
	P052[0] = iP052[0] + seg3;
	P002[0] = iP002[0] + seg4;
	P061[0] = iP061[0] + seg4;
	P069[0] = iP069[0] + seg4;
	P070[0] = iP070[0] + seg4;

	vtail += ((dtheta - vtail) * 0.1);

	if (vtail > 0.5) {
		vtail = 0.5;
	} else if (vtail < -0.5) {
		vtail = -0.5;
	}
	segup = thrash * vtail;

	P023[1] = iP023[1] + segup;
	P024[1] = iP024[1] + segup;
	P025[1] = iP025[1] + segup;
	P026[1] = iP026[1] + segup;
	P027[1] = iP027[1] + segup;
	P028[1] = iP028[1] + segup;
	P029[1] = iP029[1] + segup;
	P030[1] = iP030[1] + segup;
	P031[1] = iP031[1] + segup;
	P032[1] = iP032[1] + segup;
	P033[1] = iP033[1] + segup * 5;
	P034[1] = iP034[1] + segup * 5;
	P035[1] = iP035[1] + segup * 5;
	P036[1] = iP036[1] + segup * 5;
	P037[1] = iP037[1] + segup * 5;
	P038[1] = iP038[1] + segup * 5;
	P039[1] = iP039[1] + segup * 5;
	P040[1] = iP040[1] + segup * 5;
	P041[1] = iP041[1] + segup * 5;
	P042[1] = iP042[1] + segup * 5;
	P043[1] = iP043[1] + segup * 12;
	P044[1] = iP044[1] + segup * 12;
	P045[1] = iP045[1] + segup * 12;
	P046[1] = iP046[1] + segup * 12;
	P047[1] = iP047[1] + segup * 12;
	P048[1] = iP048[1] + segup * 12;
	P049[1] = iP049[1] + segup * 12;
	P050[1] = iP050[1] + segup * 12;
	P051[1] = iP051[1] + segup * 12;
	P052[1] = iP052[1] + segup * 12;
	P002[1] = iP002[1] + segup * 17;
	P061[1] = iP061[1] + segup * 17;
	P069[1] = iP069[1] + segup * 17;
	P070[1] = iP070[1] + segup * 17;

	glPushMatrix();

	glTranslatef(0, 0, -3000);

	glGetFloatv(GL_MODELVIEW_MATRIX, &mat[0][0]);
	n = 0;
	if (mat[0][2] >= 0) {
		n += 1;
	}
	if (mat[1][2] >= 0) {
		n += 2;
	}
	if (mat[2][2] >= 0) {
		n += 4;
	}
	glScalef(2, 1, 1);

	glEnable(GL_CULL_FACE);
	cap = wire ? GL_LINE_LOOP : GL_POLYGON;
	switch (n) {
		case 0:
			Fish_1(cap);
			break;
		case 1:
			Fish_2(cap);
			break;
		case 2:
			Fish_3(cap);
			break;
		case 3:
			Fish_4(cap);
			break;
		case 4:
			Fish_5(cap);
			break;
		case 5:
			Fish_6(cap);
			break;
		case 6:
			Fish_7(cap);
			break;
		case 7:
			Fish_8(cap);
			break;
	}
	glDisable(GL_CULL_FACE);

	glPopMatrix();
}

shark()
{
N002 = {0.000077, -0.020611, 0.999788};
N003 = {0.961425, 0.258729, -0.09339};
N004 = {0.510811, -0.769633, -0.383063};
N005 = {0.400123, 0.855734, -0.328055};
N006 = {-0.770715, 0.610204, -0.18344};
N007 = {-0.915597, -0.373345, -0.149316};
N008 = {-0.972788, 0.208921, -0.100179};
N009 = {-0.939713, -0.312268, -0.139383};
N010 = {-0.624138, -0.741047, -0.247589};
N011 = {0.591434, -0.768401, -0.244471};
N012 = {0.935152, -0.328495, -0.132598};
N013 = {0.997102, 0.074243, -0.016593};
N014 = {0.969995, 0.241712, -0.026186};
N015 = {0.844539, 0.502628, -0.184714};
N016 = {-0.906608, 0.386308, -0.169787};
N017 = {-0.970016, 0.241698, -0.025516};
N018 = {-0.998652, 0.050493, -0.012045};
N019 = {-0.942685, -0.333051, -0.020556};
N020 = {-0.660944, -0.750276, 0.01548};
N021 = {0.503549, -0.862908, -0.042749};
N022 = {0.953202, -0.302092, -0.012089};
N023 = {0.998738, 0.023574, 0.044344};
N024 = {0.979297, 0.193272, 0.060202};
N025 = {0.7983, 0.464885, 0.382883};
N026 = {-0.75659, 0.452403, 0.472126};
N027 = {-0.953855, 0.293003, 0.065651};
N028 = {-0.998033, 0.040292, 0.048028};
N029 = {-0.977079, -0.204288, 0.059858};
N030 = {-0.729117, -0.675304, 0.11114};
N031 = {0.598361, -0.792753, 0.116221};
N032 = {0.965192, -0.252991, 0.066332};
N033 = {0.998201, -0.00279, 0.059892};
N034 = {0.978657, 0.193135, 0.070207};
N035 = {0.718815, 0.680392, 0.142733};
N036 = {-0.383096, 0.906212, 0.178936};
N037 = {-0.952831, 0.29259, 0.080647};
N038 = {-0.99768, 0.032417, 0.059861};
N039 = {-0.982629, -0.169881, 0.0747};
N040 = {-0.695424, -0.703466, 0.1467};
N041 = {0.359323, -0.915531, 0.180805};
N042 = {0.943356, -0.319387, 0.089842};
N043 = {0.998272, -0.032435, 0.048993};
N044 = {0.978997, 0.193205, 0.065084};
N045 = {0.872144, 0.470094, -0.135565};
N046 = {-0.664282, 0.737945, -0.119027};
N047 = {-0.954508, 0.28857, 0.075107};
N048 = {-0.998273, 0.032406, 0.048993};
N049 = {-0.979908, -0.193579, 0.048038};
N050 = {-0.858736, -0.507202, -0.072938};
N051 = {0.643545, -0.763887, -0.048237};
N052 = {0.95558, -0.288954, 0.058068};
N058 = {0.00005, 0.793007, -0.609213};
N059 = {0.91351, 0.235418, -0.331779};
N060 = {-0.80797, 0.495, -0.319625};
N061 = {0, 0.784687, -0.619892};
N062 = {0, -1, 0};
N063 = {0, 1, 0};
N064 = {0, 1, 0};
N065 = {0, 1, 0};
N066 = {-0.055784, 0.257059, 0.964784};
N069 = {-0.000505, -0.929775, -0.368127};
N070 = {0, 1, 0};
P003 = {90, 114.73, 724.38};
P005 = {27.81, 231.52, 510.43};
P006 = {-27.81, 230.43, 509.76};
P008 = {-90, 103.84, 718.53};
P009 = {-131.1, -165.92, 834.85};
P012 = {147.96, -170.89, 845.5};
P013 = {180, 0, 2000};
P014 = {145.62, 352.67, 2000};
P015 = {55.62, 570.63, 2000};
P016 = {-55.62, 570.64, 2000};
P017 = {-145.62, 352.68, 2000};
P018 = {-180, 0.01, 2000};
P019 = {-178.2, -352.66, 2001.61};
P020 = {-55.63, -570.63, 2000};
P021 = {55.62, -570.64, 2000};
P022 = {179.91, -352.69, 1998.39};
P058 = {0, 1212.72, 2703.08};
P059 = {50.36, 0, 108.14};
P060 = {-22.18, 0, 108.14};
P062 = {516.45, -887.08, 2535.45};
P063 = {-545.69, -879.31, 2555.63};
P064 = {618.89, -1005.64, 2988.32};
P065 = {-635.37, -1014.79, 2938.68};
P066 = {0, 1374.43, 3064.18};
P071 = {-139.12, -124.96, 997.98};
P072 = {-139.24, -110.18, 1020.68};
P073 = {-137.33, -94.52, 1022.63};
P074 = {-137.03, -79.91, 996.89};
P075 = {-135.21, -91.48, 969.14};
P076 = {-135.39, -110.87, 968.76};
P077 = {150.23, -78.44, 995.53};
P078 = {152.79, -92.76, 1018.46};
P079 = {154.19, -110.2, 1020.55};
P080 = {151.33, -124.15, 993.77};
P081 = {150.49, -111.19, 969.86};
P082 = {150.79, -92.41, 969.7};
iP002 = {0, -36.59, 5687.72};
iP004 = {58.24, -146.84, 262.35};
iP007 = {-46.09, -146.83, 265.84};
iP010 = {-27.81, -285.31, 500};
iP011 = {27.81, -285.32, 500};
iP023 = {150, 0, 3000};
iP024 = {121.35, 293.89, 3000};
iP025 = {46.35, 502.93, 2883.09};
iP026 = {-46.35, 497.45, 2877.24};
iP027 = {-121.35, 293.9, 3000};
iP028 = {-150, 0, 3000};
iP029 = {-121.35, -304.84, 2853.86};
iP030 = {-46.36, -475.52, 3000};
iP031 = {46.35, -475.53, 3000};
iP032 = {121.35, -304.87, 2853.86};
iP033 = {90, 0, 4000};
iP034 = {72.81, 176.33, 4000};
iP035 = {27.81, 285.32, 4000};
iP036 = {-27.81, 285.32, 4000};
iP037 = {-72.81, 176.34, 4000};
iP038 = {-90, 0, 4000};
iP039 = {-72.81, -176.33, 4000};
iP040 = {-27.81, -285.31, 4000};
iP041 = {27.81, -285.32, 4000};
iP042 = {72.81, -176.34, 4000};
iP043 = {30, 0, 5000};
iP044 = {24.27, 58.78, 5000};
iP045 = {9.27, 95.11, 5000};
iP046 = {-9.27, 95.11, 5000};
iP047 = {-24.27, 58.78, 5000};
iP048 = {-30, 0, 5000};
iP049 = {-24.27, -58.78, 5000};
iP050 = {-9.27, -95.1, 5000};
iP051 = {9.27, -95.11, 5000};
iP052 = {24.27, -58.78, 5000};
iP061 = {0, 1181.61, 6344.65};
iP069 = {0, -418.25, 5765.04};
iP070 = {0, 1266.91, 6629.6};




P002 = {0, -36.59, 5687.72};
P004 = {58.24, -146.84, 262.35};
P007 = {-46.09, -146.83, 265.84};
P010 = {-27.81, -285.31, 500};
P011 = {27.81, -285.32, 500};
P023 = {150, 0, 3000};
P024 = {121.35, 293.89, 3000};
P025 = {46.35, 502.93, 2883.09};
P026 = {-46.35, 497.45, 2877.24};
P027 = {-121.35, 293.9, 3000};
P028 = {-150, 0, 3000};
P029 = {-152.21, -304.84, 2858.68};
P030 = {-46.36, -475.52, 3000};
P031 = {46.35, -475.53, 3000};
P032 = {155.64, -304.87, 2863.5};
P033 = {90, 0, 4000};
P034 = {72.81, 176.33, 4000};
P035 = {27.81, 285.32, 4000};
P036 = {-27.81, 285.32, 4000};
P037 = {-72.81, 176.34, 4000};
P038 = {-90, 0, 4000};
P039 = {-72.81, -176.33, 4000};
P040 = {-27.81, -285.31, 4000};
P041 = {27.81, -285.32, 4000};
P042 = {72.81, -176.34, 4000};
P043 = {30, 0, 5000};
P044 = {24.27, 58.78, 5000};
P045 = {9.27, 95.11, 5000};
P046 = {-9.27, 95.11, 5000};
P047 = {-24.27, 58.78, 5000};
P048 = {-30, 0, 5000};
P049 = {-24.27, -58.78, 5000};
P050 = {-9.27, -95.1, 5000};
P051 = {9.27, -95.11, 5000};
P052 = {24.27, -58.78, 5000};
P061 = {0, 1181.61, 6344.65};
P069 = {0, -418.25, 5765.04};
P070 = {0, 1266.91, 6629.6};
}


};
