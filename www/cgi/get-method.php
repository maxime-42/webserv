<?php

$f = fopen( 'php://stdin', 'r' );

while( $line = fgets( $f ) ) {
  echo $line;
}

fclose( $f );

echo "\n\n\n\n\n";
var_dump($_POST);
var_dump($_GET);

var_dump($_GET);
echo $_GET['submit'];
echo $_GET['firstname'];
echo $_GET['lastname'];

 if ( isset( $_GET['submit'] ) ) { 
    $firstname = $_GET['firstname'];
    $lastname = $_GET['lastname'];
    echo '<h3>Form GET Method</h3>';
    echo 'Your name is ' . $lastname . ' ' . $firstname;
    exit;
}
else
{
    echo "coucou";
}
?>
