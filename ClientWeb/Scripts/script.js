console.log('Salut mon pote');


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




//Récuperer les données du formulaire de connexion
function Connexion( ){
    
    const login = document.getElementById( "login" ).value;
    const pass  = document.getElementById( "mdp" ).value;

    console.log( login, pass )

    const crossLogin = {
        login, pass
    }

    console.table( crossLogin )



    return false;

}


//Récuperer les données du formulaire d'inscription
function Inscription( ){

    const InscriptionLog = document.getElementById( "Inlog" ).value;
    const InscriptionMdp = document.getElementById( "Inpassword" ).value;
    const mdpconfirm = document.getElementById( "Inconfirmpass" ).value;

    console.log( InscriptionLog, InscriptionMdp, mdpconfirm)
    const crossInscrit = {
        InscriptionLog, InscriptionMdp,mdpconfirm
    }

    console.table( crossInscrit )


    return false;
}

//Récuperer le message de l'utilisateur

function Message( ){
    const Messageuser = document.getElementById( "msg" ).value;

    console.log( Messageuser )

    console.table( Messageuser )

    return false;
}