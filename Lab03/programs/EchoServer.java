import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
    public static void main(String[] args) throws Exception {
        byte[] buffer = new byte[4096];
        try (ServerSocket server = new ServerSocket(5576)) {
            System.out.println("Echo server: localhost:5576");
            while (true) {
                try (Socket client = server.accept();
                     InputStream in = client.getInputStream();
                     OutputStream out = client.getOutputStream()) {
                    int count;
                    while ((count = in.read(buffer)) != -1) {
                        out.write(buffer, 0, count);
                        out.flush();
                    }
                }
            }
        }
    }
}

