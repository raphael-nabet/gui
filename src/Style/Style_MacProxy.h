// Copyright (C) 2007-2026  CEA, EDF, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File   : Style_MacProxy.h
// Purpose: macOS only - proxy over the native QMacStyle that removes the
//          "phantom" layout margins of buttons and restricts the clickable
//          area of push buttons to their visible bezel.
//
#ifndef STYLE_MACPROXY_H
#define STYLE_MACPROXY_H

#include "Style.h"

#include <QProxyStyle>

class STYLE_SALOME_EXPORT Style_MacProxy : public QProxyStyle
{
  Q_OBJECT

public:
  explicit Style_MacProxy( QStyle* base );
  virtual ~Style_MacProxy();

  // Install the proxy over the current application style if it is the
  // native macOS style. Does nothing on other platforms, if the style was
  // overridden (-style, QT_STYLE_OVERRIDE) or if the environment variable
  // SALOME_MACOS_NATIVE_STYLE is set to a non-empty value.
  static void   install();

  virtual QRect subElementRect( SubElement, const QStyleOption*, const QWidget* = 0 ) const;
  virtual void  polish( QWidget* );
  virtual void  unpolish( QWidget* );

protected:
  virtual bool  eventFilter( QObject*, QEvent* );

private:
  QRect         visibleRect( const QWidget* ) const;
};

#endif // STYLE_MACPROXY_H
