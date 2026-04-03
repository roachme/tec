#!/bin/bash
# Test runner for env and desk subcommand tests
# Runs each feature file separately to avoid test state pollution

set -e

cd "$(dirname "$0")"

echo "========================================="
echo "Running ENV and DESK Behave Tests"
echo "========================================="
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

total_passed=0
total_failed=0
total_skipped=0
failed_features=()

# Run each feature file
for feature in env-*.feature desk-*.feature; do
    echo "Testing $feature..."

    # Run behave and capture output
    if output=$(behave "$feature" --tags=-skip --format=progress 2>&1); then
        # Extract results
        passed=$(echo "$output" | grep "features passed" | awk '{print $1}')
        failed=$(echo "$output" | grep "features passed" | awk '{print $3}')
        skipped=$(echo "$output" | grep "features passed" | awk '{print $5}')

        total_passed=$((total_passed + ${passed:-0}))
        total_failed=$((total_failed + ${failed:-0}))
        total_skipped=$((total_skipped + ${skipped:-0}))

        if [ "${failed:-0}" -gt 0 ]; then
            echo -e "${RED}✗ FAILED${NC}"
            failed_features+=("$feature")
        else
            echo -e "${GREEN}✓ PASSED${NC}"
        fi
    else
        echo -e "${RED}✗ ERROR${NC}"
        failed_features+=("$feature")
        total_failed=$((total_failed + 1))
    fi
    echo ""
done

echo "========================================="
echo "Test Results Summary"
echo "========================================="
echo "Features passed:  $total_passed"
echo "Features failed:  $total_failed"
echo "Features skipped: $total_skipped"
echo ""

if [ ${#failed_features[@]} -gt 0 ]; then
    echo -e "${RED}Failed features:${NC}"
    for feature in "${failed_features[@]}"; do
        echo "  - $feature"
    done
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi
