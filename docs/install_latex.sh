#!/bin/bash
# Install LaTeX and needed deps for the documentation

VERSION="0.0.1"

echo "-- Installing LaTeX dependencies for this project v$VERSION"
echo

YES=""
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Get package manager
DNF_CMD=$(which dnf)
APT_GET_CMD=$(which apt-get)

# Packages per manager
# DNF
DNF_PACKAGE_NAMES=(
    texlive-ulem
    texlive-pgf
)
# apt-get
DEB_PACKAGE_NAMES=(
    texlive-generic-recommended
    pgf
)

# YES check
if [[ $1 == "YES" ]]; then
    YES="-y"
fi

if [[ ! -z $DNF_CMD ]]; then
    echo "Installing with DNF"
    sudo dnf check-update
    ${DIR}/latex_templates/install_latex.sh $1 # install latex deps from template
    sudo dnf install ${DNF_PACKAGE_NAMES[@]} ${YES}
elif [[ ! -z $APT_GET_CMD ]]; then
    echo "Installing with APT"
    sudo apt-get update
    ${DIR}/latex_templates/install_latex.sh $1 # install latex deps from template
    sudo apt-get install ${DEB_PACKAGE_NAMES[@]} ${YES} --no-install-recommends # No recommended docs of 500+ MB!
else
    echo "package manager not supported"
    exit 1;
fi

echo
echo "-- Done installing the dependencies"
echo
