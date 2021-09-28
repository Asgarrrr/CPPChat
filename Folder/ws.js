

const ws    = new WebSocket( "ws://127.0.0.1:1234" );
const oauth = new bootstrap.Modal( document.getElementById( "staticBackdrop" ), {
    keyboard: false
});

let user;

ws.onopen = ( ) => {

    oauth.show();

    document.getElementById( "login" ).addEventListener( "submit", ( ) => {

        const login    = document.getElementById( "logLogin" ).value
            , password = document.getElementById( "logPassword" ).value

            , req = `code:01login:${ login }password:${ password }`;

        ws.send( req );

    });

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

            console.log( "pre" )

            if ( !ID || ID[ 1 ] == 0 )
                return;

            console.log( "yesy")

            oauth.hide();
            user = code[ 1 ];
            document.getElementById( "chat" ).classList.remove( "d-none" );


        } break;

        default:
            break;
    }

};

ws.onclose = ( event ) => {

    console.log( 'Connection closed' );


}



