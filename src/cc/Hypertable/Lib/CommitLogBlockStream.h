/** -*- c++ -*-
 * Copyright (C) 2007-2012 Hypertable, Inc.
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 3 of the
 * License, or any later version.
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

#ifndef HYPERTABLE_COMMITLOGBLOCKSTREAM_H
#define HYPERTABLE_COMMITLOGBLOCKSTREAM_H

#include "Common/DynamicBuffer.h"
#include "Common/ReferenceCount.h"
#include "Common/String.h"
#include "Common/Filesystem.h"

#include "BlockCompressionHeaderCommitLog.h"


namespace Hypertable {

  class CommitLogBlockInfo {
  public:
    const char *log_dir;
    const char *file_fragment;
    uint8_t *block_ptr;
    size_t block_len;
    uint64_t start_offset;
    uint64_t end_offset;
    int error;
  };


  class CommitLogBlockStream : public ReferenceCount {

  public:

    CommitLogBlockStream(FilesystemPtr &fs);
    CommitLogBlockStream(FilesystemPtr &fs, const String &log_dir,
            const String &fragment, bool use_buffered_reader = true);
    virtual ~CommitLogBlockStream();

    void load(const String &log_dir, const String &fragment);
    void close();
    bool next(CommitLogBlockInfo *, BlockCompressionHeaderCommitLog *,
            bool refresh_length = false);

    String &get_fname() { return m_fname; }

    static bool ms_assert_on_error;

    uint64_t get_file_length() { return m_file_length; }
    uint64_t get_offset() { return m_cur_offset; }
    void refresh_length() { m_file_length = m_fs->length(m_fname); }

  private:
    int load_next_valid_header(BlockCompressionHeaderCommitLog *header);

    void reopen(uint64_t offset);

    FilesystemPtr m_fs;
    String        m_fragment;
    String        m_fname;
    String        m_log_dir;
    int32_t       m_fd;
    uint64_t      m_cur_offset;
    uint64_t      m_file_length;
    DynamicBuffer m_block_buffer;
    bool          m_use_buffered_reader;
  };

  typedef intrusive_ptr<CommitLogBlockStream> CommitLogBlockStreamPtr;

}

#endif // HYPERTABLE_COMMITLOGBLOCKSTREAM_H

