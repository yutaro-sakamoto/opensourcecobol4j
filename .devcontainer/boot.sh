#!/bin/bash

# Build and Install
./configure --prefix=/usr/
make
make install

# Set up pre-commit hook
echo << EOF
#!/bin/bash

./format
git add -u
EOF > .git/hooks/pre-commit

chmod +x .git/hooks/pre-commit