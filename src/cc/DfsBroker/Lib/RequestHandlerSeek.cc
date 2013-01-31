/**
 * Copyright (C) 2007-2012 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "Common/Compat.h"
#include "Common/Error.h"
#include "Common/Logger.h"

#include "AsyncComm/ResponseCallback.h"
#include "Common/Serialization.h"

#include "RequestHandlerSeek.h"

using namespace Hypertable;
using namespace DfsBroker;
using namespace Serialization;

/**
 *
 */
void RequestHandlerSeek::run() {
  ResponseCallback cb(m_comm, m_event);
  const uint8_t *decode_ptr = m_event->payload;
  size_t decode_remain = m_event->payload_len;

  try {
    uint32_t fd = decode_i32(&decode_ptr, &decode_remain);
    uint64_t offset = decode_i64(&decode_ptr, &decode_remain);

    m_broker->seek(&cb, fd, offset);
  }
  catch (Exception &e) {
    HT_ERROR_OUT << e << HT_END;
    cb.error(e.code(), "Error handling SEEK message");
  }
}
