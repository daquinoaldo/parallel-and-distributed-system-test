#!/usr/bin/env bash

PHI_USER=daquino-spm19
PHI_PATH=skyline

sep="\n====================================\n"

printf "${sep}Running rsync: remote path ~/${PHI_PATH}${sep}"
rsync -arzP --delete --filter=':- .gitignore' ./ ${PHI_USER}@C6320p-2.itc.unipi.it:~/${PHI_PATH}

printf "${sep}Opening a remote shell on the server.${sep}"
ssh -t ${PHI_USER}@C6320p-2.itc.unipi.it "cd ${PHI_PATH} ; make ; bash"