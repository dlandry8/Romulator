using System;
using System.IO.Ports;
using System.Text;

namespace SerialInterface
{
    public class CommPort : IDisposable
    {
        #region Constants

        /// <summary>
        /// Size of the buffer for the com port.
        /// </summary>
        private const int BUFFER_SIZE = 1024;

        #endregion

        #region Private variables

        /// <summary>
        /// Serial port used for communication.
        /// </summary>
        private SerialPort _port;

        /// <summary>
        /// Buffer for the data from the Bluetooth device.
        /// </summary>
        private byte[] _buffer = new byte[BUFFER_SIZE];

        /// <summary>
        /// Amount of data in the buffer.
        /// </summary>
        private int _currentBufferLength = 0;

        /// <summary>
        /// Character to be used as the line terminator.
        /// </summary>
        private char _lineTerminator = '\n';

        /// <summary>
        /// Used to track when Dispose is called.
        /// </summary>
        private bool _disposed = false;

        #endregion

        #region Construction and Disposal

        /// <summary>
        /// Create a new instance of the SerialComms class and open a connection
        /// using the specified connection settings.
        /// </summary>
        /// <param name="port">Serial port to open.</param>
        /// <param name="baudRate">Baud rate for teh port.</param>
        /// <param name="parity">Parity settings for the port.</param>
        /// <param name="dataBits">Number of data bits for the port.</param>
        /// <param name="stopBits">Number of stop bits for the port.</param>
        /// <param name="terminator">Character to be used as a line terminator.</param>
        public CommPort(string port, BaudRate baudRate, Parity parity, int dataBits, StopBits stopBits, char terminator = '\n')
        {
            int rate;

            _lineTerminator = terminator;
            switch (baudRate)
            {
                case BaudRate.Baudrate1200:
                    rate = 1200;
                    break;
                case BaudRate.Baudrate2400:
                    rate = 2400;
                    break;
                case BaudRate.Baudrate4800:
                    rate = 4800;
                    break;
                case BaudRate.Baudrate9600:
                    rate = 9600;
                    break;
                case BaudRate.Baudrate19200:
                    rate = 19200;
                    break;
                case BaudRate.Baudrate38400:
                    rate = 38400;
                    break;
                case BaudRate.Baudrate57600:
                    rate = 57600;
                    break;
                case BaudRate.Baudrate115200:
                    rate = 115200;
                    break;
                case BaudRate.Baudrate230400:
                    rate = 230400;
                    break;
                default:
                    rate = -1;
                    break;
            }
            _port = new SerialPort(port, rate, parity, dataBits, stopBits);
            _port.Open();
            _port.DataReceived += new SerialDataReceivedEventHandler(_port_DataReceived);
        }

        /// <summary>
        /// Implement IDisposable.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);

            // Call to GC.SupressFinalize will take this object
            // off the finalization queue and prevent multiple
            // executions.
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Initiate object disposal.
        /// </summary>
        /// <param name="disposing">Flag used to determine if the method is being called by the runtime (false) or programmatically (true).</param>
        private void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _port.Dispose();
                }
                _disposed = true;
            }
        }

        #endregion

        #region Methods

        /// <summary>
        /// Clear the buffer holding the responses from the bluetooth module.
        /// </summary>
        private void ClearBuffer()
        {
            lock (_buffer)
            {
                _buffer[0] = 0;
                _currentBufferLength = 0;
                _port.DiscardInBuffer();
            }
        }

        /// <summary>
        /// Read a line of text from the input buffer.
        /// </summary>
        /// <returns>Line of text (or empty string if no CRLF found.)</returns>
        public string ReadLine()
        {
            string result;

            result = "";
            lock (_buffer)
            {
                for (int index = 0; index < _currentBufferLength; index++)
                {
                    if (_buffer[index] == '\r')
                    {
                        _buffer[index] = 0;
                        result = "" + new string(Encoding.UTF8.GetChars(_buffer));
                        _currentBufferLength = _currentBufferLength - index - 1;
                        Array.Copy(_buffer, index + 1, _buffer, 0, _currentBufferLength);
                        break;
                    }
                }
            }
            return (result);
        }

        /// <summary>
        /// Send a line of text to the serial port.
        /// </summary>
        /// <remarks>
        /// <param name="text">Text to send to the port.</param>
        public void SendLine(string text)
        {
            ClearBuffer();
            _port.Write(Encoding.UTF8.GetBytes(text), 0, text.Length);
        }

        #endregion

        #region Events

        /// <summary>
        /// Data has been received on the com port, add it to the buffer.
        /// </summary>
        private void _port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (e.EventType == SerialData.Chars)
            {
                lock (_buffer)
                {
                    int amount;
                    byte[] buffer;

                    buffer = new byte[BUFFER_SIZE];
                    amount = ((SerialPort)sender).Read(buffer, 0, BUFFER_SIZE);
                    if (amount > 0)
                    {
                        if ((amount + _currentBufferLength) <= BUFFER_SIZE)
                        {
                            Array.Copy(buffer, 0, _buffer, _currentBufferLength, amount);
                            _currentBufferLength += amount;
                        }
                        else
                        {
                            throw new Exception("CommPort: Buffer overflow");
                        }
                    }
                }
            }
        }

        #endregion
    }
}