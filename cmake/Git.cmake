function(
  set_git_hash
  )
  find_package(Git QUIET)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE INTERNAL_GIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
    execute_process(
      COMMAND ${GIT_EXECUTABLE} remote get-url origin
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE INTERNAL_GIT_REMOTE_URL
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )
  else()
    set(INTERNAL_GIT_HASH "unknown")
    set(INTERNAL_GIT_REMOTE_URL "unknown")
  endif()
  set(GIT_HASH ${INTERNAL_GIT_HASH} PARENT_SCOPE)
  set(GIT_REMOTE_URL ${INTERNAL_GIT_REMOTE_URL} PARENT_SCOPE)
endfunction()

set_git_hash()
