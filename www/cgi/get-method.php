<?php

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
