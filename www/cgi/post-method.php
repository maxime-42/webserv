<?php

$f = fopen( 'php://stdin', 'r' );

while( $line = fgets( $f ) ) {
  echo $line;
}

fclose( $f );

var_dump($_POST);
var_dump($_GET);

echo "\n";
echo $CONTENT_LENGTH;
echo $_POST['submit'];
echo $_POST['firstname'];
echo $_POST['lastname'];

if ( isset( $_POST['submit'] ) ) { 
    $firstname = $_POST['firstname'];
    $lastname = $_POST['lastname'];
    echo '<h3>Form POST Method</h3>';
    echo 'Your name is ' . $lastname . ' ' . $firstname;
    exit; } 
else
    {
        echo "coucou";
    }
?>
