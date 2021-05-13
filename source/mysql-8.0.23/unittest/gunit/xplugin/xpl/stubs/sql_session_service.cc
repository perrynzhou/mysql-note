/* Copyright (c) 2015, 2020, Oracle and/or its affiliates.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL hereby grant you an additional
   permission to link the program and your derivative works with the
   separately licensed software that they have included with MySQL.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include "my_dbug.h"  // NOLINT(build/include_subdir)

#include "mysql/service_srv_session.h"
#include "unittest/gunit/xplugin/xpl/mock/srv_session_services.h"

namespace mock = xpl::test::mock;

// Srv_session service
//

int srv_session_init_thread(const void *plugin) {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->init_session_thread(plugin);

  DBUG_ASSERT(0);
  return 0;
}

void srv_session_deinit_thread() {
  if (mock::Srv_session::m_srv_session) {
    mock::Srv_session::m_srv_session->deinit_session_thread();
    return;
  }

  DBUG_ASSERT(0);
}

MYSQL_SESSION srv_session_open(srv_session_error_cb error_cb, void *ctxt) {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->open_session(error_cb, ctxt);

  DBUG_ASSERT(0);
  return nullptr;
}

int srv_session_close(MYSQL_SESSION session) {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->close_session(session);

  DBUG_ASSERT(0);
  return 0;
}

int srv_session_detach(MYSQL_SESSION session) {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->detach_session(session);

  DBUG_ASSERT(0);
  return 0;
}

int srv_session_attach(MYSQL_SESSION session, MYSQL_THD *old_thd) {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->attach_session(session, old_thd);

  DBUG_ASSERT(0);
  return 0;
}

int srv_session_server_is_available() {
  if (mock::Srv_session::m_srv_session)
    return mock::Srv_session::m_srv_session->server_is_available();

  DBUG_ASSERT(0);
  return 0;
}

// Srv_session info service
//

MYSQL_THD srv_session_info_get_thd(MYSQL_SESSION) {
  DBUG_ASSERT(0);
  return nullptr;
}

my_thread_id srv_session_info_get_session_id(MYSQL_SESSION session) {
  if (mock::Srv_session_info::m_srv_session_info)
    return mock::Srv_session_info::m_srv_session_info->get_session_id(session);

  DBUG_ASSERT(0);
  return 0;
}

int srv_session_info_set_client_port(Srv_session *, uint16_t) {
  DBUG_ASSERT(0);
  return 0;
}

int srv_session_info_set_connection_type(Srv_session *, enum_vio_type) {
  DBUG_ASSERT(0);
  return 0;
}

int srv_session_info_killed(MYSQL_SESSION) {
  DBUG_ASSERT(0);
  return 0;
}
