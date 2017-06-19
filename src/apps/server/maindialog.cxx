/************************************************************************/
/*                                                                      */
/*               Copyright 2008-2017 by Benjamin Seppke                 */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the GrAphical Image Processing Enviroment.   */
/*    The GRAIPE Website may be found at:                               */
/*        https://github.com/bseppke/graipe                             */
/*    Please direct questions, bug reports, and contributions to        */
/*    the GitHub page and use the methods provided there.               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>

#include "maindialog.hxx"
#include "server.hxx"

#include "core/core.h"

namespace graipe {

MainDialog::MainDialog(QWidget *parent)
:   QWidget(parent),
    m_server(NULL)
{
    Environment* env = new Environment(true);
    
    QLabel* lblStatus = new QLabel;
    lblStatus->setWordWrap(true);
    
    m_btnQuit = new QPushButton(tr("Quit"));
    m_btnQuit->setAutoDefault(false);

    m_server = new Server(env);

    if (!m_server->listen()) {
        QMessageBox::critical(this, tr("Threaded Graipe Server"),
                              tr("Unable to start the server: %1.")
                              .arg(m_server->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
     {
        if (    ipAddressesList.at(i) != QHostAddress::LocalHost
            &&  ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    
    lblStatus->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Graipe Client now.\n\nCurrent state:")
                         .arg(ipAddress).arg(m_server->serverPort()));

    connect(m_btnQuit, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_btnQuit);
    buttonLayout->addStretch(1);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lblStatus);
    mainLayout->addWidget(new QTextEdit(env->status));
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Threaded Graipe Server"));
}

} //namespace graipe
