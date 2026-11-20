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

// File   : Style_MacProxy.cxx
//
#include "Style_MacProxy.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyleOptionButton>

/*!
  \class Style_MacProxy
  \brief Proxy over the native macOS style (QMacStyle).

  QMacStyle draws the bezel of a push button smaller than the widget and
  declares extra "layout item" margins (SE_*LayoutItem) for the focus ring
  and the shadow. As a consequence the buttons are shifted by a few pixels
  in the layouts and react to clicks well outside their visible border.

  This proxy:
  - returns the widget rectangle for the SE_*LayoutItem sub-elements, so
    the layouts place the widgets exactly where they are;
  - filters the mouse presses on push buttons and ignores those located
    outside the bezel really drawn by QMacStyle;
  - hides the focus ring of the buttons.
*/

Style_MacProxy::Style_MacProxy( QStyle* base )
  : QProxyStyle( base )
{
}

Style_MacProxy::~Style_MacProxy()
{
}

void Style_MacProxy::install()
{
#ifdef __APPLE__
  if ( !QApplication::instance() || !qgetenv( "SALOME_MACOS_NATIVE_STYLE" ).isEmpty() )
    return;

  QStyle* current = QApplication::style();
  if ( !current || qobject_cast<Style_MacProxy*>( current ) )
    return;

  // apply only over the native style (not over Fusion, -style xxx, etc.)
  const bool isMacStyle = current->inherits( "QMacStyle" ) ||
    current->objectName().compare( "macintosh", Qt::CaseInsensitive ) == 0;
  if ( !isMacStyle )
    return;

  // QProxyStyle re-parents the base style to itself: QApplication::setStyle()
  // will therefore not delete it.
  QApplication::setStyle( new Style_MacProxy( current ) );
#endif
}

QRect Style_MacProxy::subElementRect( SubElement se, const QStyleOption* opt,
                                      const QWidget* w ) const
{
  switch ( se ) {
  case SE_PushButtonLayoutItem:
  case SE_ToolButtonLayoutItem:
  case SE_CheckBoxLayoutItem:
  case SE_RadioButtonLayoutItem:
  case SE_ComboBoxLayoutItem:
  case SE_SpinBoxLayoutItem:
    if ( opt )
      return opt->rect;
    break;
  default:
    break;
  }
  return QProxyStyle::subElementRect( se, opt, w );
}

void Style_MacProxy::polish( QWidget* w )
{
  QProxyStyle::polish( w );
  if ( qobject_cast<QAbstractButton*>( w ) )
    w->setAttribute( Qt::WA_MacShowFocusRect, false );
  if ( qobject_cast<QPushButton*>( w ) )
    w->installEventFilter( this );
}

void Style_MacProxy::unpolish( QWidget* w )
{
  if ( qobject_cast<QPushButton*>( w ) )
    w->removeEventFilter( this );
  QProxyStyle::unpolish( w );
}

/*!
  \brief Rectangle of the bezel really drawn by the base (native) style,
  in widget coordinates. An empty rectangle means "no restriction".
*/
QRect Style_MacProxy::visibleRect( const QWidget* w ) const
{
  const QPushButton* btn = qobject_cast<const QPushButton*>( w );
  if ( !btn || btn->isFlat() )
    return QRect();

  QStyleOptionButton opt;
  opt.initFrom( btn );
  opt.text     = btn->text();
  opt.icon     = btn->icon();
  opt.iconSize = btn->iconSize();
  opt.features = QStyleOptionButton::None;
  if ( btn->isDefault() || btn->autoDefault() )
    opt.features |= QStyleOptionButton::AutoDefaultButton;
  if ( btn->isDefault() )
    opt.features |= QStyleOptionButton::DefaultButton;
  if ( btn->menu() )
    opt.features |= QStyleOptionButton::HasMenu;

  // ask the native style, not the proxy (which returns the full rect)
  QRect r = baseStyle()->subElementRect( SE_PushButtonLayoutItem, &opt, btn );
  if ( !r.isValid() || r == opt.rect )
    return QRect();
  return r & opt.rect;
}

bool Style_MacProxy::eventFilter( QObject* o, QEvent* e )
{
  if ( e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonDblClick ) {
    QWidget* w = qobject_cast<QWidget*>( o );
    QMouseEvent* me = static_cast<QMouseEvent*>( e );
    const QRect r = visibleRect( w );
    if ( r.isValid() && !r.contains( me->pos() ) ) {
      me->ignore();
      return true; // click outside the visible bezel: swallow it
    }
  }
  return QProxyStyle::eventFilter( o, e );
}
