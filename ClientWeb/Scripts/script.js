var socket = null;

try {

    socket = new WebSocket("ws://127.0.0.1:1234");

    socket.onerror = (error) => {
        console.error(error);
    };

    socket.onopen = (event) => {

        console.log("Connexion établie.");

        // Lorsque la connexion se termine.
        this.onclose = (event) => {
            console.log("Connexion terminé.");
        };

        // Lorsque le serveur envoi un message.
        this.onmessage = (event) => {
            console.log("Message:", event.data);
        };

        this.send("Hello world!");
    };


} catch (exception) {
    console.error(exception);
}
