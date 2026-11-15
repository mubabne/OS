import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class QuoteServer {
    public static void main(String[] args) throws Exception {
        String[] quotes = {
            "Programs must be written for people to read.",
            "Simplicity is the soul of efficiency.",
            "First solve the problem, then write the code."
        };
        int next = 0;
        try (ServerSocket server = new ServerSocket(6017)) {
            System.out.println("Quote server: localhost:6017");
            while (true) {
                try (Socket client = server.accept();
                     PrintWriter out = new PrintWriter(client.getOutputStream(), true, StandardCharsets.UTF_8)) {
                    out.println(quotes[next++ % quotes.length]);
                }
            }
        }
    }
}

