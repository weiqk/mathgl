/***************************************************************************
 *   Copyright (C) 2008 by Alexey Balakin                                  *
 *   mathgl.abalakin@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include "qmglsyntax.h"
#include <mgl/parser.h>
mglParser parser;
int mgl_cmd_cmp(const void *a, const void *b);
// comment string keyword option suffix number
QColor mglColorScheme[9] = {QColor(0,127,0), QColor(255,0,0), QColor(0,0,127), QColor(127,0,0), QColor(127,0,0), QColor(0,0,255), QColor(127,0,127), QColor(0,127,127), QColor(0,0,127)};
//-----------------------------------------------------------------------------
QMGLSyntax::QMGLSyntax(QTextEdit *textEdit) : QSyntaxHighlighter(textEdit)	{}
//-----------------------------------------------------------------------------
void QMGLSyntax::highlightBlock(const QString &text)
{
	register int i, j, n, m = text.length(),s=0;
	for(n=0;parser.Cmd[n].name[0];n++){};	// determine the number of symbols in parser
	bool arg = false, nl = true;
	QString num("+-.0123456789:");
	for(i=0;i<m;i++)				// highlight paragraph
	{
		if(text[i]=='(')	s++;	if(text[i]==')')	s--;
		if(text[i]==' ' || text[i]=='\t')	continue;
		else if(text[i]=='#')	// comment
		{	setFormat(i,m-i,mglColorScheme[0]);	break;	}
		else if(text[i]=='\'')	// string
		{
			j=i;	i++;
			for(;i<m && text[i]!='\'';i++){};
			setFormat(j,i-j+1,mglColorScheme[1]);
		}
		else if(nl)				// keyword
		{
			wchar_t *s = new wchar_t[m+1];
			for(j=i;j<text.length() && !text[j].isSpace() && text[j]!=':';j++)
				s[j-i] = text[j].toLatin1();
			s[j-i]=0;
			mglCommand *rts = parser.FindCommand(s);
			if(rts)	setFormat(i,j-i+1,rts->type!=6 ? (rts->type==4 ? mglColorScheme[6] : mglColorScheme[2]) : mglColorScheme[7]);
			delete []s;
		}
		else if(text[i]==';')	{	arg = true;	nl = false;	continue;	}
		else if(text[i]==':' && s==0)	{	nl=true;	continue;	}
		else if(arg)			// option
		{
			const char *o[]={"xrange","yrange","zrange","crange","alpha",
							"cut","ambient","meshnum","fontsize","alphadef",
							"marksize","legend"};
			int l[12] = {6, 6, 6, 6, 5, 3, 7, 7, 8, 8, 8, 6};
			for(j=0;j<12;j++)
				if(text.indexOf(o[j],i)==i && (i+l[j]==text.length() || text[i+l[j]].isSpace()))
					setFormat(i,l[j],mglColorScheme[3]);
		}
		else if(text[i]=='.' && i+1<text.length() && text[i+1].isLetter())	// suffix
		{
			for(j=i;j<text.length() && !text[j].isSpace();j++){};
			setFormat(i,j-i+1,mglColorScheme[4]);
		}
		else if(num.contains(text[i]))	// number
			setFormat(i,1,mglColorScheme[5]);
		else if((text[i]=='e' || text[i]=='E') && i+1<text.length() && num.contains(text[i-1]) && num.contains(text[i+1]) )
			setFormat(i,1,mglColorScheme[5]);
		else		// number as its symbolic id
		{
			const char *o[]={"nan","pi","on","off"};
			int l[4] = {3, 2, 2, 3};
			for(j=0;j<4;j++)
				if(text.indexOf(o[j],i)==i && (i+l[j]==text.length() || text[i+l[j]].isSpace()))
					setFormat(i,l[j],mglColorScheme[5]);
		}
		arg = nl = false;
	}
}
//-----------------------------------------------------------------------------
MessSyntax::MessSyntax(QTextEdit *textEdit) : QSyntaxHighlighter(textEdit)	{}
//-----------------------------------------------------------------------------
void MessSyntax::highlightBlock(const QString &text)
{
	if(text.left(7) == ("In line"))
		setFormat(0, text.length(), QColor(255,0,0));
}
//-----------------------------------------------------------------------------
