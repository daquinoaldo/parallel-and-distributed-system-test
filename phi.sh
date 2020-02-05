#!/usr/bin/env bash

PHI_PATH=skyline
PHI_USER=daquino-spm19

sep="\n====================================\n"
printf "${sep}Running rsync: remote path ~/${PHI_PATH}${sep}"

rsync -arzP --delete --filter=':- .gitignore' --exclude .gitignore --exclude phi.sh \
    ./ ${PHI_USER}@C6320p-2.itc.unipi.it:~/${PHI_PATH}

printf "${sep}Opening a remote shell on the server.${sep}"

ssh -t ${PHI_USER}@C6320p-2.itc.unipi.it \
  "cd ${PHI_PATH} ; \
  cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ . ; \
  make ; \
  bash"
