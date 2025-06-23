#!/usr/bin/env python3
import os
import sys
import base64

base_dir = os.path.abspath(os.path.dirname(__file__))
ICON_DIR = os.path.join(base_dir, "..", "data", "icons")
INJECT_MARKER = "<!-- inject:icons -->"

# Generate the header content, so that the HTML page is saved in PROGMEM, and save it to file
def generate_header_from_string(html, dest_path):
    with open(dest_path, "w", encoding="utf-8") as f:
        f.write('#pragma once\n\n')
        f.write('const char settings_html[] PROGMEM = R"rawliteral(\n')
        f.write(html)
        f.write('\n)rawliteral";\n')

# Retrieve all icons in the icon folder
def load_svg_icons_base64():
    icons = {}
    for filename in os.listdir(ICON_DIR):
        if filename.endswith(".svg"):
            varname = os.path.splitext(filename)[0]
            with open(os.path.join(ICON_DIR, filename), "rb") as f:
                encoded = base64.b64encode(f.read()).decode("utf-8")
                icons[varname] = f"data:image/svg+xml;base64,{encoded}"
    return icons

# Generate JS code describing an object containing all icons
def generate_icons_script_base64(icons):
    lines = ['<script>', 'const icons = {']
    for name, data_uri in icons.items():
        lines.append(f'  "{name}": "{data_uri}",')
    lines.append('};')
    lines.append('</script>')
    return "\n".join(lines)

# Main function: retrieve the file path, inject SVG icons, pass it to generate_header
def main():
    src = os.path.join(base_dir, "..", "data", "settings.html")
    dest = os.path.join(base_dir, "..", "include", "settings_html.h")

    if not os.path.exists(src):
        print(f"Source HTML file not found: {src}")
        sys.exit(1)

    # Read HTML
    with open(src, "r", encoding="utf-8") as f:
        html = f.read()

    # Inject base64 SVGs
    icons = load_svg_icons_base64()
    icon_script = generate_icons_script_base64(icons)
    html = html.replace(INJECT_MARKER, icon_script)

    # Output header
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    generate_header_from_string(html, dest)

if __name__ == "__main__":
    main()
