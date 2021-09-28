


const ws    = new WebSocket( "ws://192.168.65.103:4457" );
const oauth = new bootstrap.Modal( document.getElementById( "staticBackdrop" ), {
    keyboard: false
});

let user;

ws.onopen = ( ) => {

    oauth.show();

    document.getElementById( "login" ).addEventListener( "submit", async ( ) => {

        const login    = document.getElementById( "logLogin" ).value
            , password = document.getElementById( "logPassword" ).value

            , req = `code:01login:${ await sha256( login ) }password:${ await sha256( password ) }`;

        ws.send( req );

        return false;

    });

    document.getElementById( "register" ).addEventListener( "submit", async ( ) => {

        const login    = document.getElementById( "regLogin" ).value
            , password = document.getElementById( "regPassword" ).value
            , pseudo   = document.getElementById( "regPseudo" ).value

            , req = `code:02login:${ await sha256( login ) }password:${ await sha256( password ) }pseudo:${ pseudo }`;

        ws.send( req );

        return false;

    });

    document.getElementById( "send" ).addEventListener( "submit", ( ) => {

        const input = document.getElementById( "messageContent" );

        if( !user || !input.value || !input.value.length )
            return;
        ws.send( `code:03ID:${ user.id }message:${ input.value }` );

        input.value = "";

    })

};

ws.onmessage = ( event ) => {

    // —— Try to get the code
    const code = event.data.match( /code:(\d{0,2})/ );

    console.log( code );

    // —— If the code is not found
    if ( !code )
        return;

    switch ( code[ 1 ] ) {

        case "01" : {

            const ID = event.data.match( /ID:(\d+)/ );

            console.log( ID )

            if ( !ID || ID[ 1 ] == 0 )
                return;

            console.log( "yesy")

            oauth.hide();
            user = {
                id      : ID[ 1 ],
                name    : document.getElementById( "logLogin" ).value,
            }
            document.getElementById( "chat" ).classList.remove( "d-none" );


        } break;

        case "03" : {

            if ( !user )
                return;

            const feed = document.getElementById( "messageFeed" )

            const newMessage = document.createElement( "li" );
            const messageContent = event.data.match( /pseudo:(.+)message:(.+)/ );

            newMessage.classList.add( "messsageL" );

            if ( !messageContent )
                return;


            newMessage.innerHTML = `
                <div class="message">
                    <div class="message-header">
                        <img src="https://avatars.dicebear.com/api/identicon/${messageContent[ 1 ]}.svg" class="avatar" alt="">
                    </div>
                    <div class="message-body">
                        <b>${ messageContent[ 1 ] } :</b><br/>
                        ${ messageContent[ 2 ] }
                    </div>
                </div>`;

            feed.appendChild( newMessage );

            feed.scrollTo( 0, feed.scrollHeight);

        } break;

        default:
            break;
    }

};

ws.onclose = ( event ) => {

    console.log( 'Connection closed', event );


}

async function sha256(message) {

    // -- Encode as UTF-8
    const msgBuffer = new TextEncoder().encode(message);

    // -- Hash the message
    const hashBuffer = await crypto.subtle.digest('SHA-256', msgBuffer);

    // -- Convert ArrayBuffer to Array
    const hashArray = Array.from(new Uint8Array(hashBuffer));

    // -- Convert bytes to hex string ( zi bordel )
    return hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
}