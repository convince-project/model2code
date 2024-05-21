# Configuration file for the Sphinx documentation builder.
from cgitb import html
# -- Project information

project = 'MODEL2CODE'
copyright = '2024'
author = 'CONVINCE Consortium'

release = '0.1'
version = '0.1.0'

# -- General configuration
# Breathe/exhale configuration
breathe_projects = {"model2code":"../build/doxygen/xml"}
breathe_default_project = "model2code"

exhale_args = {
        "containmentFolder"    : "./API",
        "rootFileName"         : "api.rst",
        "rootFileTitle"        : "API",
        "doxygenStripFromPath" : "..",
        "createTreeView": True
    }

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp' 

extensions = [
    'sphinx.ext.autosummary',
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'breathe',
    'exhale'
]

# intersphinx_mapping = {
#     'python': ('https://docs.python.org/3/', None),
#     'sphinx': ('https://www.sphinx-doc.org/en/master/', None),
#     'networkx': ('https://networkx.org/documentation/stable/', None),
# }
intersphinx_disabled_domains = ['std']

templates_path = ['_templates']

# -- Options for HTML output

html_theme = 'sphinx_rtd_theme'
html_logo = 'convince_logo_horizontal_200p.png'
html_theme_options = {
    # 'analytics_id': 'G-XXXXXXXXXX',  #  Provided by Google in your dashboard
    # 'analytics_anonymize_ip': False,
    # 'logo_only': False,
    # 'display_version': True,
    # 'prev_next_buttons_location': 'bottom',
    # 'style_external_links': False,
    # 'vcs_pageview_mode': '',
    # 'style_nav_header_background': '#FF5555',
    # Toc options
    # 'collapse_navigation': True,
    # 'sticky_navigation': True,
    # 'navigation_depth': 99,
    # 'includehidden': True,
    # 'titles_only': False
}
html_static_path = ['_static']
# html_css_files = [
#     'css/custom.css',
# ]
html_style = 'css/custom.css'

# -- Options for EPUB output
epub_show_urls = 'footnote'

# autodoc2_packages = [
#     "../../jani_generator/src/jani_generator",
# ]