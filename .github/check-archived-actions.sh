#!/usr/bin/env bash
# Warns when .github/workflows/ uses an action whose upstream repository is archived.
#
# Run it from anywhere; it only needs the gh CLI to be authenticated.
# It never fails the build: an upstream archival is a heads-up, not a defect of
# the commit under test, so every exit path is 0.

set -uo pipefail
shopt -s nullglob

# Run from the repository root so that annotation paths stay repo-relative,
# which is what GitHub needs to attach them to the file in the diff view.
cd "$(dirname "$0")/.." || {
  echo "::warning::check-archived-actions.sh could not reach the repository root."
  exit 0
}
workflow_dir=".github/workflows"

workflow_files=("$workflow_dir"/*.yml "$workflow_dir"/*.yaml)
if [ "${#workflow_files[@]}" -eq 0 ]; then
  echo "No workflow files under $workflow_dir."
  exit 0
fi

# owner/repo of every remote action, ignoring local reusable workflows (./...),
# container actions (docker://...) and expressions (${{ ... }}). The leading
# anchor keeps commented-out steps and `uses:` inside run: blocks out of it.
mapfile -t repos < <(grep -rhoE '^[[:space:]]*-?[[:space:]]*uses:[[:space:]]*"?'"'"'?[^[:space:]"'"'"'#]+' "${workflow_files[@]}" \
  | sed -E 's/^[[:space:]]*-?[[:space:]]*uses:[[:space:]]*["'"'"']?//' \
  | grep -vE '^(\./|docker://|\$\{\{)' \
  | cut -d@ -f1 \
  | cut -d/ -f1,2 \
  | sort -u)

archived_count=0
unknown_count=0
summary_header_written=""

for repo in "${repos[@]}"; do
  archived="$(gh api "repos/$repo" --jq '.archived' 2>/dev/null)"
  case "$archived" in
    true) ;;
    false)
      continue
      ;;
    *)
      # The query failed: deleted or renamed repository, rate limit, network,
      # missing token. Report the count so that a check that has quietly
      # stopped working cannot look like a clean result.
      unknown_count=$((unknown_count + 1))
      continue
      ;;
  esac

  archived_count=$((archived_count + 1))
  pattern="$(printf '%s' "$repo" | sed 's/\./\\./g')"

  grep -rnE "^[[:space:]]*-?[[:space:]]*uses:[[:space:]]*[\"']?$pattern([/@])" "${workflow_files[@]}" \
    | cut -d: -f1,2 \
    | while IFS=: read -r file line; do
        echo "::warning file=$file,line=$line::$repo is archived upstream. It no longer receives updates and Dependabot cannot bump it. Consider migrating to a maintained action."
      done

  if [ -n "${GITHUB_STEP_SUMMARY:-}" ]; then
    if [ -z "$summary_header_written" ]; then
      echo "### Archived actions" >> "$GITHUB_STEP_SUMMARY"
      summary_header_written=1
    fi
    echo "- \`$repo\` is archived upstream" >> "$GITHUB_STEP_SUMMARY"
  fi
done

echo "Checked ${#repos[@]} action(s): $archived_count archived, $unknown_count undetermined."

if [ "$unknown_count" -ne 0 ]; then
  echo "::warning::Could not determine the archive status of $unknown_count action(s). The repository may have been deleted or renamed, or the API query failed."
fi

exit 0
