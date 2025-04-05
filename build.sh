set -xe
gcc adm.c adme.c pasmlexer.c -o adme -Wall -Wextra
gcc pasm.c pasmlexer.c pasmparser.c adm.c -o pasm -Wall -Wextra