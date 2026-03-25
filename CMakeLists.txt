set(SRCS
    src/addr.c
    src/agent.c
    src/base64.c
    src/conn.c
    src/conn_poll.c
    src/conn_thread.c
    src/conn_mux.c
    src/const_time.c
    src/crc32.c
    src/hash.c
    src/hmac.c
    src/ice.c
    src/juice.c
    src/log.c
    src/random.c
    src/server.c
    src/stun.c
    src/timestamp.c
    src/tcp.c
    src/turn.c
    src/udp.c
    src/compat.c
)

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS include
    PRIV_INCLUDE_DIRS src include/juice
    PRIV_REQUIRES mbedtls esp_netif
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    USE_NETTLE=0
    USE_MBEDTLS=1
    JUICE_STATIC
    JUICE_EXPORTS
)

# Kconfig-driven definitions
if(CONFIG_JUICE_ICE_MAX_CANDIDATES)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_ICE_MAX_CANDIDATES_COUNT=${CONFIG_JUICE_ICE_MAX_CANDIDATES})
endif()
if(CONFIG_JUICE_TASK_STACK_SIZE)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_TASK_STACK_SIZE=${CONFIG_JUICE_TASK_STACK_SIZE})
endif()
if(CONFIG_JUICE_MAX_STUN_SERVERS)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_MAX_SERVER_ENTRIES_COUNT=${CONFIG_JUICE_MAX_STUN_SERVERS})
endif()
if(CONFIG_JUICE_MAX_RELAY_SERVERS)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_MAX_RELAY_ENTRIES_COUNT=${CONFIG_JUICE_MAX_RELAY_SERVERS})
endif()
if(CONFIG_JUICE_STUN_MAX_CREDENTIAL_LEN)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_STUN_MAX_CREDENTIAL_LEN=${CONFIG_JUICE_STUN_MAX_CREDENTIAL_LEN})
endif()
if(CONFIG_JUICE_BUFFER_SIZE)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_BUFFER_SIZE=${CONFIG_JUICE_BUFFER_SIZE})
endif()
if(CONFIG_JUICE_LOG_BUFFER_SIZE)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        JUICE_LOG_BUFFER_SIZE=${CONFIG_JUICE_LOG_BUFFER_SIZE})
endif()
if(NOT CONFIG_JUICE_ENABLE_SERVER)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE NO_SERVER)
endif()

# Rename symbols that conflict with lwIP and wpa_supplicant
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    udp_sendto=juice_udp_sendto
    udp_sendto_self=juice_udp_sendto_self
    udp_recvfrom=juice_udp_recvfrom
    udp_create_socket=juice_udp_create_socket
    udp_get_port=juice_udp_get_port
    udp_get_bound_addr=juice_udp_get_bound_addr
    udp_get_local_addr=juice_udp_get_local_addr
    udp_get_addrs=juice_udp_get_addrs
    udp_set_diffserv=juice_udp_set_diffserv
    hmac_sha1=juice_hmac_sha1
    hmac_sha256=juice_hmac_sha256
    hash_md5=juice_hash_md5
    hash_sha1=juice_hash_sha1
    hash_sha256=juice_hash_sha256
)
