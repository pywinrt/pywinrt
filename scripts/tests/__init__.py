import sys
from pathlib import Path

# The scripts import one another by their bare names, as they do when they are
# run as files, so a test that imports one needs their directory on sys.path.
sys.path.insert(0, str(Path(__file__).parent.parent))
