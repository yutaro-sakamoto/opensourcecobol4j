import { defineConfig } from 'vitepress'

const repo = 'https://github.com/yutaro-sakamoto/Hagane-COBOL'

// The site is served from https://yutaro-sakamoto.github.io/Hagane-COBOL/
export default defineConfig({
  title: 'Hagane COBOL',
  base: '/Hagane-COBOL/',
  cleanUrls: true,
  // The Javadoc of libcobj is copied into /javadoc/libcobj/ after the build.
  ignoreDeadLinks: [/\/javadoc\//],
  lastUpdated: false,
  head: [['link', { rel: 'icon', href: '/Hagane-COBOL/favicon.svg' }]],

  themeConfig: {
    logo: '/favicon.svg',
    socialLinks: [{ icon: 'github', link: repo }],
    search: { provider: 'local' },
  },

  locales: {
    root: {
      label: 'English',
      lang: 'en',
      description: 'A COBOL compiler that translates COBOL programs to Java programs',
      themeConfig: {
        nav: [
          { text: 'Get Started', link: '/guide/linux', activeMatch: '/guide/' },
          { text: 'Javadoc', link: '/javadoc/libcobj/index.html', target: '_self' },
          { text: 'Releases', link: `${repo}/releases` },
        ],
        sidebar: [
          {
            text: 'Installation',
            items: [
              { text: 'Linux', link: '/guide/linux' },
              { text: 'Windows', link: '/guide/windows' },
              { text: 'Docker', link: '/guide/docker' },
            ],
          },
          {
            text: 'Guide',
            items: [{ text: 'Usage', link: '/guide/usage' }],
          },
        ],
        footer: {
          message: 'libcobj is released under the LGPL v3. Other parts are released under the GPL v3.',
        },
      },
    },
    ja: {
      label: '日本語',
      lang: 'ja',
      link: '/ja/',
      description: 'COBOLプログラムをJavaプログラムに変換するCOBOLコンパイラ',
      themeConfig: {
        nav: [
          { text: 'はじめる', link: '/ja/guide/linux', activeMatch: '/ja/guide/' },
          { text: 'Javadoc', link: '/javadoc/libcobj/index.html', target: '_self' },
          { text: 'リリース', link: `${repo}/releases` },
        ],
        sidebar: [
          {
            text: 'インストール',
            items: [
              { text: 'Linux', link: '/ja/guide/linux' },
              { text: 'Windows', link: '/ja/guide/windows' },
              { text: 'Docker', link: '/ja/guide/docker' },
            ],
          },
          {
            text: 'ガイド',
            items: [{ text: '使い方', link: '/ja/guide/usage' }],
          },
        ],
        footer: {
          message: 'libcobjはLGPL v3、それ以外はGPL v3の下で配布されています。',
        },
        outline: { label: '目次' },
        docFooter: { prev: '前のページ', next: '次のページ' },
        darkModeSwitchLabel: '外観',
        sidebarMenuLabel: 'メニュー',
        returnToTopLabel: 'トップに戻る',
        langMenuLabel: '言語',
      },
    },
  },
})
