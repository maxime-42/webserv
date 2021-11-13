#!/bin/bash

mkdir dir_no_access_rights && chmod 000 dir_no_access_rights
echo "<html><head><title style=\"color:tomato;\" >THAT SHOULD NOT HAPPEN</title></head>
<body>
<center><h1></h1></center>
<hr><center style=\"color:tomato;\" >THAT SHOULD NOT HAPPEN</center>
</body>
</html>" > files/file_no_access_rights.html
chmod 000 files/file_no_access_rights.html
