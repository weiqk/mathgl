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
#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QTabWidget>
#include "mgl/parser.h"
#include "mgl/qt.h"
#include "info_dlg.h"
extern mglParser parser;
//-----------------------------------------------------------------------------
InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent)
{
	var = 0;	kz = 0;		allowRefresh = true;
	QWidget *p;
	QVBoxLayout *v, *u;
	QLabel *l;
	QTabWidget *tab = new QTabWidget(this);
	u = new QVBoxLayout(this);	u->addWidget(tab);
	// graphics
	p = new QWidget(this);	v = new QVBoxLayout(p);
	l = new QLabel(tr("Select kind of plot"),this);	v->addWidget(l);
	kind = new QComboBox(this);	v->addWidget(kind);
	mgl = new QMathGL(this);	v->addWidget(mgl,1);
	mgl->autoResize = true;		mgl->appName = tr("Data preview");
	mgl->setToolTip(tr("Data preview for current slice."));

	draw = new mglDrawScript(&parser);	mgl->setDraw(draw);

	kind->addItem(tr("1D plot"));	kind->addItem(tr("2D plot"));
	kind->setCurrentIndex(0);	//	kind->addItem(tr("3D plot"));
	connect(kind, SIGNAL(currentIndexChanged(int)), this, SLOT(refresh()));
	kind->setToolTip(tr("Kind of plots: lines for 1D, density for 2D."));
	tab->addTab(p, tr("Preview"));
	// information
	info = new QTextEdit(this);
	info->setToolTip(tr("Short information about the data."));
	tab->addTab(info, tr("Information"));
	connect(mgl,SIGNAL(showWarn(QString)),info,SLOT(setText(QString)));
}
//-----------------------------------------------------------------------------
InfoDialog::~InfoDialog()	{	delete draw;	}
//-----------------------------------------------------------------------------
#include <QMessageBox>
void InfoDialog::refresh(bool force)
{
	if(!var || (!force && (!allowRefresh || !isVisible())))	return;
	QString text, name, sub;
	name = QString::fromStdWString(var->s);
	sub = "(:,:,"+QString::number(kz)+")\n";
	int i = kind->currentIndex();
	if(i<1)	text = "yrange "+name+"\nplot "+name + sub;
	else	text = "crange "+name+"\ndens "+name + sub;
	text = "zoom 0.15 0.15 0.85 0.85\nbox\n" + text + "\ninfo "+name;
	draw->text = text;	mgl->update();
}
//-----------------------------------------------------------------------------
void InfoDialog::setVar(mglVar *v)
{
	var=v;
	if(v)	kind->setCurrentIndex(v->d.ny>1 ? 1:0);
	refresh();
}
//-----------------------------------------------------------------------------
void InfoDialog::showEvent(QShowEvent *)
{	refresh(true);	}
//-----------------------------------------------------------------------------