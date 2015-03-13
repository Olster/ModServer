#ifndef PLUGIN_API_DATA_CHUNK_H_
#define PLUGIN_API_DATA_CHUNK_H_

// DataChunk is passed to the plugin when the connection receives data.
class DataChunk {
 public:
  DataChunk()
   : m_next(0) {
    m_buffer[0] = '\0';
  }

  DataChunk(const DataChunk& other) = delete;
  DataChunk& operator=(const DataChunk& other) = delete;

  virtual ~DataChunk() {}

  char* buf_writable() { return m_buffer; }
  int buf_size() { return m_bufSize; }
 private:
  DataChunk* m_next;
  static const int m_bufSize = 2048;
  char m_buffer[m_bufSize];
};

#endif  // PLUGIN_API_DATA_CHUNK_H_
