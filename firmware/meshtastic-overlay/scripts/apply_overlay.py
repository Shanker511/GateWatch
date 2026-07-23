#!/usr/bin/env python3
from pathlib import Path
import argparse
import shutil
import sys


def copy_tree(source: Path, target: Path) -> None:
    for item in source.rglob('*'):
        rel = item.relative_to(source)
        destination = target / rel
        if item.is_dir():
            destination.mkdir(parents=True, exist_ok=True)
        else:
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(item, destination)


def patch_modules(modules_cpp: Path) -> None:
    text = modules_cpp.read_text(encoding='utf-8')

    include_block = '#ifdef GATEWATCH_NODE\n#include "modules/GateWatchModule.h"\n#endif\n'
    if include_block not in text:
        marker = '#include "configuration.h"\n'
        if marker not in text:
            raise RuntimeError('Could not find configuration include in Modules.cpp')
        text = text.replace(marker, marker + include_block, 1)

    setup_block = '#ifdef GATEWATCH_NODE\n    new GateWatchModule();\n#endif\n'
    if setup_block not in text:
        marker = 'routingModule = new RoutingModule();'
        if marker not in text:
            raise RuntimeError('Could not find RoutingModule creation in Modules.cpp')
        text = text.replace(marker, setup_block + '    ' + marker, 1)

    modules_cpp.write_text(text, encoding='utf-8', newline='\n')


def main() -> int:
    parser = argparse.ArgumentParser(description='Apply GateWatch files to a Meshtastic firmware checkout')
    parser.add_argument('meshtastic_root', type=Path)
    args = parser.parse_args()

    upstream = args.meshtastic_root.resolve()
    overlay = Path(__file__).resolve().parents[1]

    if not (upstream / 'src/modules/Modules.cpp').exists():
        print(f'Not a Meshtastic firmware checkout: {upstream}', file=sys.stderr)
        return 2

    copy_tree(overlay / 'variants', upstream / 'variants')
    copy_tree(overlay / 'src', upstream / 'src')
    patch_modules(upstream / 'src/modules/Modules.cpp')
    print(f'GateWatch overlay applied to {upstream}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
