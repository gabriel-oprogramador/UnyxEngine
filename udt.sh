#!/usr/bin/env bash
# Unyx Development Tool

set -e
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

main(){ 
  make -f "$ROOT/Build/Rules/Makefile" "$@"
}
main "$@"
