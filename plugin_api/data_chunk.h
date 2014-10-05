#ifndef PLUGIN_API_DATA_CHUNK_H_
#define PLUGIN_API_DATA_CHUNK_H_

// DataChunk is passed to the plugin when the connection receives data.
class DataChunk {
 public:
  DataChunk() {}
  ~DataChunk() {}

  char* buf_writable() { return m_buffer; }
  int buf_size() { return m_bufSize; }
 private:
  DataChunk* Next;
  static const int m_bufSize = 512;
  char m_buffer[m_bufSize];
};

#endif  // PLUGIN_API_DATA_CHUNK_H_
