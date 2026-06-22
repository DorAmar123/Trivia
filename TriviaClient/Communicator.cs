using System.Net.Sockets;

namespace TriviaClient;

public class Communicator
{
    private TcpClient? m_client;
    private NetworkStream? m_stream;

    private readonly object m_ioLock = new object();

    private const string SERVER_IP = "127.0.0.1";
    private const int SERVER_PORT = 8826;
    private const int IO_TIMEOUT_MS = 10000;

    public void Connect()
    {
        m_client = new TcpClient();
        m_client.Connect(SERVER_IP, SERVER_PORT);
        m_stream = m_client.GetStream();
        m_stream.ReadTimeout = IO_TIMEOUT_MS;
        m_stream.WriteTimeout = IO_TIMEOUT_MS;
    }

    public void Disconnect()
    {
        m_stream?.Close();
        m_client?.Close();
        m_stream = null;
        m_client = null;
    }

    public bool IsConnected()
    {
        return m_client != null && m_client.Connected;
    }

    public void SendRequest(byte[] packet)
    {
        if (m_stream == null)
            throw new Exception("Not connected to server");

        m_stream.Write(packet, 0, packet.Length);
    }

    public ServerResponse SendAndReceive(byte[] packet)
    {
        lock (m_ioLock)
        {
            SendRequest(packet);
            return ReceiveResponse();
        }
    }

    private ServerResponse ReceiveResponse()
    {
        if (m_stream == null)
            throw new Exception("Not connected to server");

        // Read header: 1 byte code + 4 bytes size
        byte[] header = ReadBytes(5);

        int size = (header[1] << 24) | (header[2] << 16) | (header[3] << 8) | header[4];

        byte[] fullPacket;
        if (size > 0)
        {
            byte[] body = ReadBytes(size);
            fullPacket = new byte[5 + size];
            Array.Copy(header, fullPacket, 5);
            Array.Copy(body, 0, fullPacket, 5, size);
        }
        else
        {
            fullPacket = header;
        }

        return Deserializer.Deserialize(fullPacket);
    }

    private byte[] ReadBytes(int count)
    {
        byte[] buf = new byte[count];
        int total = 0;
        while (total < count)
        {
            int read = m_stream!.Read(buf, total, count - total);
            if (read == 0)
                throw new Exception("Server disconnected");
            total += read;
        }
        return buf;
    }
}
