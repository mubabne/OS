import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class HaikuServer {
    public static void main(String[] args) throws Exception {
        try (ServerSocket server = new ServerSocket(5575)) {
            System.out.println("Haiku server: localhost:5575");
            while (true) {
                try (Socket client = server.accept();
                     PrintWriter out = new PrintWriter(client.getOutputStream(), true, StandardCharsets.UTF_8)) {
                    out.println("Soft dawn wakes the code");
                    out.println("Threads run under the kernel");
                    out.println("Tasks sleep, then return");
                }
            }
        }
    }
}
