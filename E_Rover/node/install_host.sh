source env.sh
cd ${SRC_PILOT_FOLDER}
ssh ${GUEST_USER}@${GUEST_IP} -p ${GUEST_PORT} 'bash -s' < install_remote.sh storage/external-1

