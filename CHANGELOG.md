# [3.0.0-alpha.1](https://github.com/lc-soft/LCUI/compare/v3.0.0-alpha.0...v3.0.0-alpha.1) (2026-11-22)

### Bug Fixes

* **app:** replace steptimer-based render throttle with interval gate ([d6fdae1](https://github.com/lc-soft/LCUI/commit/d6fdae1feb2f781fd8b2a52527f9a5029a3081f2))
* build for string-render example ([#331](https://github.com/lc-soft/LCUI/issues/331)) ([d212185](https://github.com/lc-soft/LCUI/commit/d212185924b4783a5d6a94ad44617704afca93ba))
* Consola -> Consolas ([e716f48](https://github.com/lc-soft/LCUI/commit/e716f482334e8988d1433828d420b273d72f0645))
* **css:** destroy temporary parsed style values ([eb793c5](https://github.com/lc-soft/LCUI/commit/eb793c59590ce7294f34c899e4c80b179c0697e6))
* **css:** property table coverage should compare int values ([e92a4d3](https://github.com/lc-soft/LCUI/commit/e92a4d39067546c043d1fed76470b3886623c59e))
* **css:** report unsupported declarations at debug level ([279eacd](https://github.com/lc-soft/LCUI/commit/279eacd3ada9db2c6a505115ad762eb83d0a97fd))
* **css:** reset parse target on '}' to avoid stale property name ([3d2a8dc](https://github.com/lc-soft/LCUI/commit/3d2a8dc35a16f69edaf1bb5f0d2f69c5ee85020f))
* **css:** stack buffer overflow in selector fullname construction ([259c6ad](https://github.com/lc-soft/LCUI/commit/259c6ad696fd381d32b1644cca2c6f264b3560a6))
* delay refresh window to use the latest size of widget ([1de2dfd](https://github.com/lc-soft/LCUI/commit/1de2dfd62c2f6b33058a4618694725cf758da0fc))
* **doc-viewer:** demo code tabs should show a visible hover background ([0b63de7](https://github.com/lc-soft/LCUI/commit/0b63de799f1f49ebf1017eb3294898aa92cd79f9))
* **doc-viewer:** demo language tabs should not wrap onto two rows ([c649626](https://github.com/lc-soft/LCUI/commit/c6496268c0ab65bdecd0c8d410e57af161300285))
* **doc-viewer:** rebuild responsive home gallery ([3cce29c](https://github.com/lc-soft/LCUI/commit/3cce29caa4d4c05de403dacb0ab80fd105d6de3e))
* **examples:** make the fabric demo DPI-aware and fit the window ([3270d8f](https://github.com/lc-soft/LCUI/commit/3270d8f34f289bfbbf11ac41b281b57a9c099c22))
* **font:** use the resolved family name for fontconfig aliases ([a9d94e2](https://github.com/lc-soft/LCUI/commit/a9d94e21888c31baf819e208d5b157ff3c3fdb50))
* format-overflow error in gcc 14 ([8f205ec](https://github.com/lc-soft/LCUI/commit/8f205eca97c33f4cb28d7f2a7841b69f0d6a11f3))
* keep example tests in the console subsystem on Windows ([ce47e50](https://github.com/lc-soft/LCUI/commit/ce47e50ed3ca0c962d70078714ee99e61b9d5b5f))
* **lcui_ui:** window resize should update the bound content ([1ae26ca](https://github.com/lc-soft/LCUI/commit/1ae26cab65a496cb9e4196aaa746ca279f759fc6))
* make fontconfig work again ([60fe643](https://github.com/lc-soft/LCUI/commit/60fe64305809144501feb9556dd5a1f2f608aa10))
* make window resize conversion explicit ([cf7ec0c](https://github.com/lc-soft/LCUI/commit/cf7ec0c0eae497d48d90dc4bbc6a818366e6c3c8))
* **pandagl:** free unused fallback glyph bitmaps ([2b3c33a](https://github.com/lc-soft/LCUI/commit/2b3c33a2460f8d54243ccd46f16744ee45874dcf))
* **pandagl:** implicit declaration of function ‘pd_rect’ ([890fb49](https://github.com/lc-soft/LCUI/commit/890fb49536c1a75633450036dce6b1210ce1ccdd))
* **pandagl:** nested [color] tags should use innermost tag's color ([8e539a9](https://github.com/lc-soft/LCUI/commit/8e539a9ccc547c5411c81432891fb7ead83fb79b))
* **pandagl:** release fontconfig global caches on shutdown ([7404070](https://github.com/lc-soft/LCUI/commit/7404070d5e5d349ab5d224c58cc86fd8a8f2aa33))
* **pandagl:** text deletion crashes and pd_text_dump missing line breaks ([18a98e5](https://github.com/lc-soft/LCUI/commit/18a98e5bd68985fecd6bb0de88ea679a4a7d3982)), closes [#270](https://github.com/lc-soft/LCUI/issues/270)
* **pandagl:** unify text baseline across fallback fonts ([711296f](https://github.com/lc-soft/LCUI/commit/711296f718c28dbcccb3e36ebb8d0f6b0ef5b99e))
* **pandagl:** zero glyph bitmap metrics on init ([ed25199](https://github.com/lc-soft/LCUI/commit/ed25199836b6f8205f0d90bc070be7f29e6e86c3))
* **ptk:** emit window size event synchronously on Windows ([e26cd95](https://github.com/lc-soft/LCUI/commit/e26cd958a03e318ae3e7d4623237d063f264a645))
* **ptk:** fix window size constraints mapping ([cf23a96](https://github.com/lc-soft/LCUI/commit/cf23a967bb20925d47b076cab1e77f12b1fcb241))
* **ptk:** handle window creation failures on win32 ([926b725](https://github.com/lc-soft/LCUI/commit/926b7252d177aac8d47aed001b9c21cb8f807de2))
* **ptk:** handler self-removal during dispatch should not cause crash ([fa35037](https://github.com/lc-soft/LCUI/commit/fa35037c1a9203b0c70eeba6e47743885496872c))
* **ptk:** ignore special characters entered with Ctrl key combination ([d3197db](https://github.com/lc-soft/LCUI/commit/d3197db4254147e1573d1b3042bc9d235b79dec7))
* **ptk:** incorrect window minmaxinfo ([ae9b228](https://github.com/lc-soft/LCUI/commit/ae9b228f48e2e6f9fd449400d78a244ba56b506d))
* **ptk:** initialize steptimer last_time to wall clock ([a0a1669](https://github.com/lc-soft/LCUI/commit/a0a1669176e3d9d4acb0109d5db587dd0fa721a9))
* **ptk:** keep cached window size in sync on Windows ([87d63f3](https://github.com/lc-soft/LCUI/commit/87d63f3c8ad6968d54803b7054b03c0aa1c7117b))
* **ptk:** remove cross-process window assertion ([0c842a1](https://github.com/lc-soft/LCUI/commit/0c842a10b8c96222bb84c875694d4edf42d5489f))
* **ptk:** request text failed when clipboard content is empty ([#318](https://github.com/lc-soft/LCUI/issues/318)) ([a9ab79a](https://github.com/lc-soft/LCUI/commit/a9ab79a338cf33dc81e8e73265e277d80768c1ef))
* **ptk:** ui_server should not bypass platform window destruction flow ([9fd4bdb](https://github.com/lc-soft/LCUI/commit/9fd4bdb89f41a2d963b41e88da16caa0855f6fdd))
* **ptk:** X11 window geometry should reflect requested values ([01b022b](https://github.com/lc-soft/LCUI/commit/01b022bd4cbee9610662dfea43617dd94aee1452))
* **router:** remove unused libui dependency ([af3c76c](https://github.com/lc-soft/LCUI/commit/af3c76c2394e45177dfa4befd06995baf47e08ba))
* **router:** reuse matched components on sibling navigation ([78b401c](https://github.com/lc-soft/LCUI/commit/78b401c1b5f08ae93986a7ec9acb61a242d6fdcb))
* **styles:** correct card action and title class selectors ([331e078](https://github.com/lc-soft/LCUI/commit/331e07840a6472f6d969ec299fae2814d21a54e1))
* **styles:** dark button variants should stay flat ([1a6c310](https://github.com/lc-soft/LCUI/commit/1a6c3100e2d8c0b91e3eef5ce223779334f5fc56))
* **styles:** keep the focus ring out of grouped inputs ([1a030c3](https://github.com/lc-soft/LCUI/commit/1a030c3a4817d2327eed3330ae92a1e3b15934ef))
* **styles:** remove unsupported letter-spacing from dropdown shortcut ([5460d5a](https://github.com/lc-soft/LCUI/commit/5460d5a733a0885b8f2ea187e67d1d119ee12c98))
* UI display mode should be reset after destruction ([31c6f19](https://github.com/lc-soft/LCUI/commit/31c6f199c0e0ec71772afd5592147f9be707748c))
* **ui-server:** apply window resize before the first frame ([7c72628](https://github.com/lc-soft/LCUI/commit/7c72628f23669be6b54577102a2fa180318654f5))
* **ui-server:** correct inverted strcmp for update_visible flag ([7952ff2](https://github.com/lc-soft/LCUI/commit/7952ff2508f9b98aca9118a8e96e5c48b4a910e5))
* **ui:** avoid mbstowcs assertion in ui_print_tree on Windows ([3f49931](https://github.com/lc-soft/LCUI/commit/3f49931cb6ecef97b561e248bd12c5b2b34c6c0b))
* **ui:** calculate widget offsets without duplicate padding ([4801465](https://github.com/lc-soft/LCUI/commit/4801465d9382d678f138170661e0bca0008c1445))
* **ui:** cannot set css shorthand property ([c4895a3](https://github.com/lc-soft/LCUI/commit/c4895a3efb6910f79c15afa543b60412e472cc0a))
* **ui:** clear implicit conversion warnings ([06a958e](https://github.com/lc-soft/LCUI/commit/06a958eda47850663642132a46df45e6ed55626a))
* **ui:** debug log should not keep an unused style pointer ([ee6fc19](https://github.com/lc-soft/LCUI/commit/ee6fc1956df035e6653f84238f4bccc803b7e61b))
* **ui:** destroy temporary string style values ([53ef6ac](https://github.com/lc-soft/LCUI/commit/53ef6ac477b9ed5268ea391e6498a031e05a61a7))
* **ui:** event initialization should clear uninitialized mouse data ([ac01108](https://github.com/lc-soft/LCUI/commit/ac011085ec8ecab1ca65ea86cf42c05280b2d8d3))
* **ui:** image listener removal should free list nodes ([0e0bf7b](https://github.com/lc-soft/LCUI/commit/0e0bf7b17947d7c9afa9a9525a7663e840117384))
* **ui:** incorporate children intrinsic width into cross size calculation ([3c6a592](https://github.com/lc-soft/LCUI/commit/3c6a592e98fc3079c0d1313284cc4845c240966b))
* **ui:** make fit-content width and tooltip reveal deterministic ([18b74cc](https://github.com/lc-soft/LCUI/commit/18b74cc9f18d0e35db40a52b92c8056d2d24b024))
* **ui:** memory leak and division by zero error ([ded5f84](https://github.com/lc-soft/LCUI/commit/ded5f840222659b9359dcb04acee1889611f177a))
* **ui:** percentage flex items should wrap within intrinsic size ([8cb5bd7](https://github.com/lc-soft/LCUI/commit/8cb5bd7c320facffef3eb371946b28fd84482fdc))
* **ui:** refresh canvas box when overflow styles change ([6f3348c](https://github.com/lc-soft/LCUI/commit/6f3348cb07f59f7201cd041cf421759870ab6a96))
* **ui:** refresh status styles only for matched selectors ([cbf051e](https://github.com/lc-soft/LCUI/commit/cbf051e0613a5bc7217de0de3222e4e6aff0c1e9))
* **ui:** serialize image loading and start the loader after init ([8666284](https://github.com/lc-soft/LCUI/commit/86662849bd94b514a18308f7ab8127b87711cfaa))
* **ui:** size flex lines by content in fixed-size containers ([5b18848](https://github.com/lc-soft/LCUI/commit/5b18848ef97438d1c90c9c699aa693d92fe49346))
* **ui:** stabilize flex layout and add layout contract tests ([20b3221](https://github.com/lc-soft/LCUI/commit/20b3221cfdd99db1ad88a24f941f21174fb393a2))
* **ui:** stabilize layout measurement and style refresh ([1f46778](https://github.com/lc-soft/LCUI/commit/1f4677889347db7ab5e7de2b030a2ceba6c6ec0d))
* **ui:** ui_widget_render should re-anchor offscreen captures in logical units ([f760db2](https://github.com/lc-soft/LCUI/commit/f760db2176203bde589418c5d149beac2ba9f15b))
* **ui:** wrap and stabilize flex column text layout ([22c883b](https://github.com/lc-soft/LCUI/commit/22c883b79eed9a4eb6f7190257f6044653704f2b))
* **widgets:** bind late data dialog triggers ([7e5350a](https://github.com/lc-soft/LCUI/commit/7e5350a8832bc808c309ac7bb737a92f28f1bff6))
* **widgets:** ceil text bbox size to avoid rounding precision loss ([b6a2ad5](https://github.com/lc-soft/LCUI/commit/b6a2ad54da6675d3e804b4d94441a10aec85df80))
* **widgets:** center button text vertically ([c85a159](https://github.com/lc-soft/LCUI/commit/c85a1592b9d0d1bc158ce90f8b7e29e77bf0d6ca))
* **widgets:** contain nested scrollarea wheel events ([8c78437](https://github.com/lc-soft/LCUI/commit/8c78437264d05dd92eb606bf03f7ecb64fb9f35e))
* **widgets:** fix password mask buffer overflow ([51706db](https://github.com/lc-soft/LCUI/commit/51706dbfb20a5e7044a96e97a4e3e506d9e47f63))
* **widgets:** invalidate portal scroll parents on destroy ([6667d19](https://github.com/lc-soft/LCUI/commit/6667d19e63a31027202b92798dd70219b3f238cc))
* **widgets:** keep the sidebar footer menu in flow ([b7a5779](https://github.com/lc-soft/LCUI/commit/b7a5779b61d4016f1a5372881c8a399ecf7c07cb))
* **widgets:** snapshot text source in ui_text_set_content_w ([9d5d8d5](https://github.com/lc-soft/LCUI/commit/9d5d8d5a1ddd5fd913bb10befc070e4285f5dde6))

### Code Refactoring

* **app:** drop app_ prefix from public API ([20b24cb](https://github.com/lc-soft/LCUI/commit/20b24cb9fc2ddce4f3fa95540bf98167e7220c97))
* **yutil:** rename public APIs and migrate timer to ptk ([3801fdc](https://github.com/lc-soft/LCUI/commit/3801fdc10fcad7b0d918c4d3b28ae6b6fc2b5f4d))

### Features

* add explicit window lifecycle and per-window event routing ([093a6a1](https://github.com/lc-soft/LCUI/commit/093a6a1e351c203ad4a92158f07aff9ee4b122da))
* add progress widget ([2ccb6aa](https://github.com/lc-soft/LCUI/commit/2ccb6aa1517978fa15eb5742d3c95d7c8b5b9428))
* add window icon support ([b0c031f](https://github.com/lc-soft/LCUI/commit/b0c031f8dc680efdf1229ceed9365d9892f0702d))
* **app:** add lcui_request_frame and animation timer driver ([e6506fc](https://github.com/lc-soft/LCUI/commit/e6506fca907cc2b41591043a3682903e01e8ed98))
* **css:** add align-self support for flex items ([8558552](https://github.com/lc-soft/LCUI/commit/855855263bc3c8b4287b13f72ee406556b2c1af8))
* **css:** add CSS property inheritance support ([604e922](https://github.com/lc-soft/LCUI/commit/604e9226e85b8a35688e349715fd7f90247aa399))
* **css:** add gap, row-gap and column-gap support ([f54cd12](https://github.com/lc-soft/LCUI/commit/f54cd125396282b9a14949ec663a8eccdfd69daa))
* **css:** add text-decoration property ([ae5b7d2](https://github.com/lc-soft/LCUI/commit/ae5b7d2ed9d67b6b29e5b837220abecc43560352))
* **css:** allow unloading style rules by source ([64ea750](https://github.com/lc-soft/LCUI/commit/64ea75088591dfe52c63b84588d55023e7acaf63))
* **css:** support CSS Color 4 rgb alpha syntax ([812391f](https://github.com/lc-soft/LCUI/commit/812391f1f5e00fb95375acdbae48ea2e24a08ef7))
* **css:** support cursor property with standard keyword values ([8448023](https://github.com/lc-soft/LCUI/commit/844802374d43a011f1000f6b7fef7cc73a9b6a96))
* **ctest:** add suite filter, list and CTEST_MAIN macro ([c6cb7da](https://github.com/lc-soft/LCUI/commit/c6cb7da777d55cb5b7821d1eac543c3a93a66125))
* **doc-viewer:** localize and rebalance the home gallery ([e87f0f1](https://github.com/lc-soft/LCUI/commit/e87f0f1f068cd3df445846fa65048ce01ebeb2ef))
* **doc-viewer:** rebuild navigation and docs route ([718bd2d](https://github.com/lc-soft/LCUI/commit/718bd2d540858e62360bfb29f347c9f1d2a4ebed))
* **doc-viewer:** show repository icons, star counts and version badge ([2c4958b](https://github.com/lc-soft/LCUI/commit/2c4958b4275993f14b3853302677171d81ed80d5))
* **fonts:** load system UI/CJK fonts and register generic family aliases ([ee08bcf](https://github.com/lc-soft/LCUI/commit/ee08bcf9f91cd317551e473b24429b78fb5b45ae))
* **pandagl:** add backslash escape for style tag brackets ([4a79f41](https://github.com/lc-soft/LCUI/commit/4a79f417d30172e1c32d31c0646644ac8d4726b6))
* **pandagl:** add pd_canvas_rotate and pd_canvas_mask_composite ([ad0ce70](https://github.com/lc-soft/LCUI/commit/ad0ce709ee96de7f561ab203e163147564746de3))
* **pandagl:** add quiet font loading for optional fonts ([7449e6f](https://github.com/lc-soft/LCUI/commit/7449e6fb4a85ce036e4db3d24bd4378e112fc922))
* **pandagl:** add text underline render support ([11f8c66](https://github.com/lc-soft/LCUI/commit/11f8c66a659d21c49b302a71bc15d6dd9fc3a4d4))
* **pandagl:** support variable fonts and fix font fallback ([218f900](https://github.com/lc-soft/LCUI/commit/218f900a58247aa6dd164686ab1b29fec3ff4da7))
* **ptk:** add per-window cursor api for win32, x11 and fb ([237a5b6](https://github.com/lc-soft/LCUI/commit/237a5b66f236d4487b8d8887e8092283afb457ea))
* **ptk:** add Windows implemention for clipboard ([a69f2b1](https://github.com/lc-soft/LCUI/commit/a69f2b1bd7c3d32d6445cb936217c7c6a5524dfd))
* **settings:** add custom string settings API ([d48c8cc](https://github.com/lc-soft/LCUI/commit/d48c8cc259a0571f99503b4da3e321c4f9bc91a4))
* **settings:** schema-driven serialization and window state persistence ([4bb0d51](https://github.com/lc-soft/LCUI/commit/4bb0d51cd54fdae5106f0812048efaf7d3517527))
* **styles:** add class variants for badge, alert, item and card ([83c1c6d](https://github.com/lc-soft/LCUI/commit/83c1c6d07d73a84d058ce0843d25b8319fe7b873))
* **styles:** add light and dark theme support ([9217b1c](https://github.com/lc-soft/LCUI/commit/9217b1ca3a2e679fd5de214444f0796d8fd80f45))
* **styles:** add switchable accent color themes ([dc0ab38](https://github.com/lc-soft/LCUI/commit/dc0ab38546b7c59ee3f466687c49be60a0394b2e))
* **styles:** centralize widget styles in the nova default theme ([bbdb80f](https://github.com/lc-soft/LCUI/commit/bbdb80f045cd60278cfa1fde49672c3741af41fd))
* **styles:** follow the accent in focus rings and app fills ([747a222](https://github.com/lc-soft/LCUI/commit/747a222fcc82dbf73e4cddfd70c5f15b3a8c80ce))
* **ui-server:** sync system cursor with hovered widget style ([708611a](https://github.com/lc-soft/LCUI/commit/708611a727e19dd478f12783c097c5b663429670))
* **ui:** propagate inherited CSS values to dependent children ([251bf2f](https://github.com/lc-soft/LCUI/commit/251bf2fa543eb1f393daca48da063a172664bb35))
* **ui:** render text underlines with offset and thickness control ([916aac4](https://github.com/lc-soft/LCUI/commit/916aac4850eeacb14cfa64fa22426f9cb2d6df87))
* update settings APIs and add persistent configuration support ([#329](https://github.com/lc-soft/LCUI/issues/329)) ([cc0b79f](https://github.com/lc-soft/LCUI/commit/cc0b79fccbfa73108b9a41ce766e25b736117185))
* **widgets:** add accordion widget ([3dda91b](https://github.com/lc-soft/LCUI/commit/3dda91be1187cc51f3af4a14751c436202101e15))
* **widgets:** add alert widget ([b674004](https://github.com/lc-soft/LCUI/commit/b674004673298a731924fbfda70b591cb0103c4a))
* **widgets:** add attachment widget ([49e7199](https://github.com/lc-soft/LCUI/commit/49e719995cc319ca4c95357d936ca7111aa790db))
* **widgets:** add badge widget ([6b7a4d4](https://github.com/lc-soft/LCUI/commit/6b7a4d41dea9f55ab5ec2bf4ad4dc790ef03187c))
* **widgets:** add breadcrumb widget ([263c62e](https://github.com/lc-soft/LCUI/commit/263c62e531dfae1023644732b1e90aa2acc2b4c2))
* **widgets:** add button group widget ([029d5c4](https://github.com/lc-soft/LCUI/commit/029d5c4e221b9703e6e62d17e86f1377860d1cfa))
* **widgets:** add button variants, sizes and themed styles with docs ([7154337](https://github.com/lc-soft/LCUI/commit/715433794424d99b4c7cdf21adf4b1751ac36676))
* **widgets:** add card widget ([860b1f1](https://github.com/lc-soft/LCUI/commit/860b1f128955881403dbc610908a94dc8b86b93e))
* **widgets:** add checkbox widget ([8f77b1f](https://github.com/lc-soft/LCUI/commit/8f77b1f32e68d979ac523fe85016c93d07cb8505))
* **widgets:** add class based button styles ([91444c6](https://github.com/lc-soft/LCUI/commit/91444c60cd2eff705c9a86a39a8fbb8b204fe61c))
* **widgets:** add collapsible widget ([898a96c](https://github.com/lc-soft/LCUI/commit/898a96cb8cff36c44487efeb749aa943a54eb7d1))
* **widgets:** add dialog and alert-dialog widgets ([8876c70](https://github.com/lc-soft/LCUI/commit/8876c70f93cc4a787e9357d41209594b4e5ffe98))
* **widgets:** add dropdown menu trigger width matching ([9071235](https://github.com/lc-soft/LCUI/commit/907123560a87e2279e237a016a3b46ace41237ac))
* **widgets:** add dropdown menu widget ([ba50322](https://github.com/lc-soft/LCUI/commit/ba50322e2232046ae08848c422c725cd3c15597a))
* **widgets:** add empty widget ([20ac448](https://github.com/lc-soft/LCUI/commit/20ac4484b9d5fb8d4871cbd680c039b272a9e0b2))
* **widgets:** add field widget ([13c32f9](https://github.com/lc-soft/LCUI/commit/13c32f98a35378606cf279106f6241ef16ca9593))
* **widgets:** add groups, scrolling, alignment and checkmark to select ([323b641](https://github.com/lc-soft/LCUI/commit/323b641bbc63b02e64a53d854f2da24067f8b805))
* **widgets:** add icon widget ([92b46ee](https://github.com/lc-soft/LCUI/commit/92b46eee101da534e8dc331c3e51fcf96e218683))
* **widgets:** add input group widget ([a23c16f](https://github.com/lc-soft/LCUI/commit/a23c16f5c534932951680d00e273d83d8c4df16b))
* **widgets:** add item widget ([a805852](https://github.com/lc-soft/LCUI/commit/a8058525589c2a91a5f61cc229ee66d9676a92da))
* **widgets:** add label widget ([38aab01](https://github.com/lc-soft/LCUI/commit/38aab01ec64f7809eac2703162302dab032f5b5d))
* **widgets:** add popover widget ([47014c5](https://github.com/lc-soft/LCUI/commit/47014c5dbb3d43b02abd178e05601cc4edfd2dc2))
* **widgets:** add portal and select widget ([10b3665](https://github.com/lc-soft/LCUI/commit/10b36650711e1c35759ee8b85fe6e3453b55516c))
* **widgets:** add progress label and value ([1e48b88](https://github.com/lc-soft/LCUI/commit/1e48b886a0bc744c8fe4e793aa08d6242615d95c))
* **widgets:** add radio-group widget ([fd23dd0](https://github.com/lc-soft/LCUI/commit/fd23dd079f1ac6543b77ea053d1ccdb2302bc7e5))
* **widgets:** add sidebar widget ([f226404](https://github.com/lc-soft/LCUI/commit/f2264045549d5bc3a6f3396a05182e49633076e8))
* **widgets:** add spinner and shimmer widgets ([a1078ff](https://github.com/lc-soft/LCUI/commit/a1078ff20150a0816af4e57a511c1cd3124191b4))
* **widgets:** add toast widget ([ccad5ef](https://github.com/lc-soft/LCUI/commit/ccad5ef13186bbb815065dab5ee9c961d480694c))
* **widgets:** add tooltip widget ([2ac38a6](https://github.com/lc-soft/LCUI/commit/2ac38a6ca64860a16df907b7f6314d2f9991be46))
* **widgets:** migrate textinput editing to the stb_textedit kernel ([90dbba2](https://github.com/lc-soft/LCUI/commit/90dbba2cddcdddbf4b85998ca4632aeb268dfeed))
* **widgets:** restyle scrollbar with a slim rounded thumb ([de31a3b](https://github.com/lc-soft/LCUI/commit/de31a3bc748f099fead316488225c78b237cef61))
* **widgets:** support multiline text input attribute ([e857ce1](https://github.com/lc-soft/LCUI/commit/e857ce1245a002aee96ac5755d5368be91d46478))
* **widgets:** update field style ([e80b997](https://github.com/lc-soft/LCUI/commit/e80b997c4516632feb9fc3d164a02eea05c2060d))
* **widgets:** update radio-group style ([16f5e91](https://github.com/lc-soft/LCUI/commit/16f5e91cb86cd91253f9e4549a3117080342ebad))

### BREAKING CHANGES

* **yutil:** all renamed yutil public APIs require downstream
    to update function names. Deleted APIs require code changes:
    - get_time_us: use y_gettime() for ms timestamps
    - get_time_delta: use y_gettime() - start instead
    - sleep_s: use y_sleep(ms) with millisecond argument
    - wcsreplace: no replacement, had zero callers
    - cmdsplit: WinMain now uses __argc/__argv from CRT
    - timer_list_*: migrated to ptk timer_scheduler_*;
      ptk_reset_timer still available for external use
* **app:** lcui_app_get_fps -> lcui_get_fps,
  lcui_app_set_frame_rate_cap -> lcui_set_fps_cap,
  settings.frame_rate_cap -> settings.fps_cap.
  lcui_app_init, lcui_app_destroy, lcui_app_process_events
  removed from public header — use lcui_init, lcui_destroy,
  lcui_run instead.

# [3.0.0-alpha.0](https://github.com/lc-soft/LCUI/compare/v2.2.0...v3.0.0-alpha.0) (2025-01-10)
### Changelog (Updated with Present Tense)

- Change naming style to `snake_case`  
- Switch to using XMake as the build tool
- Restructure architecture into multiple sub-libraries:  
    - `lib/yutil`  
    - `lib/pandagl`  
    - `lib/css`  
    - `lib/i18n`  
    - `lib/ptk`  
    - `lib/thread`  
    - `lib/router`  
    - `lib/ui-router`  
    - `lib/worker`  
    - `lib/ui`  
    - `lib/ui-xml`  
    - `lib/ui-cursor`  
    - `lib/ui-server`  
- Redesign the interfaces of LCUI and its sub-libraries  
- Introduce the boolean type defined in `stdbool.h`  
- **css:** Optimize data structures to reduce memory usage  
- **css:** Add a value definition syntax parser  
- **css:** Update property parsers  
- **css:** Add support for registering custom properties  
- **css:** Add support for the escape character `\`  
- **css:** Add shorthand property `background`  
- **css:** Add shorthand property `border`  
- **css:** Add shorthand properties for `border-top/right/bottom/left`  
- **css:** Add the `background-clip` property  
- **css:** Add the `inline-flex` keyword  
- **worker:** Add support for canceling unexecuted tasks  
- **pandagl:** Add support for setting font family aliases  
- **ptk:** Add clipboard module  
- **ptk:** Add DPI awareness  
- **ui:** Add UIMutationObserver
- **ui:** Improve asynchronous image loading and cache management  
- **ui:** Improve widet update processes  
- **ui:** Improve layout calculation processes  
- **ui:** Improve dirty rectangle collection  
- **ui-server:** Add support for binding widets to system windows  
- **ui-server:** Add per-window DPI awareness  
- **lcui/ui:** Improve fullscreen mode  
- **lcui/widgets:** Add `scrollarea` widet and improve the working mode of the `scrollbar` widet  
- **lcui/widgets:** Add `router-link` and `router-view` widets  
- **lcui/widgets:** Change file URI format to `"file:///path/to/file"`  
- **lcui/widgets:** Rename `textview` widet to `text`  
- **lcui/widgets:** Rename `textedit` widet to `textinput`  

# [2.2.0](https://github.com/lc-soft/LCUI/compare/v2.1.0...v2.2.0) (2021-05-30)


### Bug Fixes

* conditional jump or move depends on uninitialised value(s) ([7174868](https://github.com/lc-soft/LCUI/commit/717486861541b93e5ab95a246dfce90650d4273c))
* **gui:** the horizontal scroll bar does not work ([#219](https://github.com/lc-soft/LCUI/issues/219)) ([31dee24](https://github.com/lc-soft/LCUI/commit/31dee2494d7e5c88af02052084f962cf71797141))
* **gui:** the size of the absolutely positioned widget is incorrect ([35bfa3f](https://github.com/lc-soft/LCUI/commit/35bfa3f0e7d92c53f95fa9825217d115df8c74c0))
* **gui:** the style of hidden widgets will not be updated ([f0a6e30](https://github.com/lc-soft/LCUI/commit/f0a6e30a6d85d466cbfffc5fd0c2e91aacb574c9))
* **image:** incorrect image suffix detection ([46095e7](https://github.com/lc-soft/LCUI/commit/46095e70e39e5f1440fb1b5b9effaa35b76c6bbe))
* **util:** ‘NULL’ is undeclared when the function of dict is inlined ([c9c9901](https://github.com/lc-soft/LCUI/commit/c9c990167d44da9ca54fd3fed5753567aade1825))
* remove the incorrect `CSSParser_GetRuleParser()` macro ([3bd6b71](https://github.com/lc-soft/LCUI/commit/3bd6b715729255607760abadbf72c22d8c2681c8))
* variable naming conflict in LCUI_PostSimpleTask() ([f0382d4](https://github.com/lc-soft/LCUI/commit/f0382d435a5b0d19a14165101605e7f021191e8a))


### Features

* **font:** add operation functions for TextStyle ([c0ccdf8](https://github.com/lc-soft/LCUI/commit/c0ccdf81e5404fc21cd804ec95e0a030800a75fc))



# [2.1.0](https://github.com/lc-soft/LCUI/compare/v2.0.0...v2.1.0) (2020-07-05)


### Bug Fixes

* wasm-ld: error: duplicate symbol: self ([2b0424c](https://github.com/lc-soft/LCUI/commit/2b0424c))
* **display:** rect_array is not freed ([224c433](https://github.com/lc-soft/LCUI/commit/224c433))
* **gui:** invalid area are not added after the widget is destroyed ([8e1f136](https://github.com/lc-soft/LCUI/commit/8e1f136))
* **gui:** segmention fault in TextView_SetTextW() ([a10df88](https://github.com/lc-soft/LCUI/commit/a10df88)), closes [#195](https://github.com/lc-soft/LCUI/issues/195)
* **ime:** segmentation fault when set caret ([0b644e1](https://github.com/lc-soft/LCUI/commit/0b644e1))
* **linux:** segmentation fault on draw window with minimal size ([#200](https://github.com/lc-soft/LCUI/issues/200)) ([#201](https://github.com/lc-soft/LCUI/issues/201)) ([fa23f89](https://github.com/lc-soft/LCUI/commit/fa23f89))
* **mainloop:** move out paint event trigger from openmp parallel loop ([#204](https://github.com/lc-soft/LCUI/issues/204)) ([#205](https://github.com/lc-soft/LCUI/issues/205)) ([6978f46](https://github.com/lc-soft/LCUI/commit/6978f46))


### Features

* add settings api ([#191](https://github.com/lc-soft/LCUI/issues/191)) ([#211](https://github.com/lc-soft/LCUI/issues/211)) ([#212](https://github.com/lc-soft/LCUI/issues/212)) ([1b8c6e6](https://github.com/lc-soft/LCUI/commit/1b8c6e6))

# [2.0.0](https://github.com/lc-soft/LCUI/compare/v1.3.0...v2.0.0) (2020-03-02)


### Bug Fixes

* **builder:** comment node should be ignore ([460ee00](https://github.com/lc-soft/LCUI/commit/460ee00))
* **css:** conditional jump or move depends on uninitialised value(s) ([44486f1](https://github.com/lc-soft/LCUI/commit/44486f1))
* **font:** fix to get the correct file path ([#187](https://github.com/lc-soft/LCUI/issues/187)) ([6d54685](https://github.com/lc-soft/LCUI/commit/6d54685))
* **font:** FontBitmap_Free() memory leak ([c47a6c0](https://github.com/lc-soft/LCUI/commit/c47a6c0))
* **gui:** root widget is missing hover and active status ([02c03c7](https://github.com/lc-soft/LCUI/commit/02c03c7))
* **gui:** segmentation fault on resize empty window ([#199](https://github.com/lc-soft/LCUI/issues/199)) ([56ce0b5](https://github.com/lc-soft/LCUI/commit/56ce0b5))
* **gui:** TextCaret did not remove the timer after destruction ([a58b12d](https://github.com/lc-soft/LCUI/commit/a58b12d))
* **linux:** InitLinuxKeybord() memory leak bug ([61cadc8](https://github.com/lc-soft/LCUI/commit/61cadc8))
* **timer:** the callback will still run after the timer is removed ([43233b3](https://github.com/lc-soft/LCUI/commit/43233b3))
* correct the use of variables ([513b3b9](https://github.com/lc-soft/LCUI/commit/513b3b9))
* **windows:** touch support is not enabled by default ([bc7710a](https://github.com/lc-soft/LCUI/commit/bc7710a))
* cursor position should be updated before handle widget events ([83fc949](https://github.com/lc-soft/LCUI/commit/83fc949))
* **util:** the preset dict types are not exported correctly ([20abb19](https://github.com/lc-soft/LCUI/commit/20abb19))


### Code Refactoring

* **gui:** add widget_background.h ([d69fbb0](https://github.com/lc-soft/LCUI/commit/d69fbb0))
* **gui:** add widget_border.h ([843232e](https://github.com/lc-soft/LCUI/commit/843232e))
* **gui:** add widget_shadow.h ([08ed51c](https://github.com/lc-soft/LCUI/commit/08ed51c))
* **gui:** improve widget update processing ([1a50aec](https://github.com/lc-soft/LCUI/commit/1a50aec))


### Features

* **builder:** output details when an error occurs ([f7ed3b8](https://github.com/lc-soft/LCUI/commit/f7ed3b8))
* **css:** add flexbox property parser ([07d2911](https://github.com/lc-soft/LCUI/commit/07d2911))
* **display:** add LCUIDisplay_EnablePaintFlashing() ([298ffa4](https://github.com/lc-soft/LCUI/commit/298ffa4))
* **display:** flashing rendered rects ([#180](https://github.com/lc-soft/LCUI/issues/180)) ([#190](https://github.com/lc-soft/LCUI/issues/190)) ([5ad4fec](https://github.com/lc-soft/LCUI/commit/5ad4fec))
* **display:** set the minimum screen size to 320x240 ([317df70](https://github.com/lc-soft/LCUI/commit/317df70))
* **gui:** add CSSFontStyle_IsEquals() ([80d4149](https://github.com/lc-soft/LCUI/commit/80d4149))
* **gui:** add flexible box layout ([3cbb246](https://github.com/lc-soft/LCUI/commit/3cbb246))
* **gui:** change the call timing and parameter list for the runtask() ([f058916](https://github.com/lc-soft/LCUI/commit/f058916))
* **gui:** rewrite widget layout system ([24e89aa](https://github.com/lc-soft/LCUI/commit/24e89aa))
* **gui:** ScrollBar widget will reset when the container is destroyed ([61e0f2c](https://github.com/lc-soft/LCUI/commit/61e0f2c))
* **gui:** set the container padding when the ScrollBar is visible ([a436f41](https://github.com/lc-soft/LCUI/commit/a436f41))
* **gui:** unwrap() will fire the link and unlink events for each child ([48344bc](https://github.com/lc-soft/LCUI/commit/48344bc))
* **gui:** update the sizing rules for the TextView widget ([ea7e9d2](https://github.com/lc-soft/LCUI/commit/ea7e9d2))
* **util:** use inline functions instead of global variables ([0991d14](https://github.com/lc-soft/LCUI/commit/0991d14))


### Performance Improvements

* **display:** no need to add dirty rectangles after resizing ([b0985a2](https://github.com/lc-soft/LCUI/commit/b0985a2))
* **display:** OpenMP is enabled only when the render area is large ([1e57d9d](https://github.com/lc-soft/LCUI/commit/1e57d9d))
* **gui:** improve invalid area collection ([e9ea262](https://github.com/lc-soft/LCUI/commit/e9ea262))
* **gui:** improve textedit update processing ([bbb7cbc](https://github.com/lc-soft/LCUI/commit/bbb7cbc))
* **gui:** improve textview update processing ([6824735](https://github.com/lc-soft/LCUI/commit/6824735))
* add OpenMP support for widget rendering ([#118](https://github.com/lc-soft/LCUI/issues/118)) ([#189](https://github.com/lc-soft/LCUI/issues/189)) ([d858333](https://github.com/lc-soft/LCUI/commit/d858333))
* improve dirty rectangle collection ([c81da29](https://github.com/lc-soft/LCUI/commit/c81da29))


### BREAKING CHANGES

* **util:** `DictType_StringKey` and `DictType_StringCopyKey` have been replaced by inline functions
* **display:** LCUIDisplay_ShowRectBorder() LCUIDisplay_HideRectBorder() have been removed.
* **gui:** The widget shadow operation interface has been changed to private
* **gui:** The widget background operation interface has been changed to private
* **gui:** The widget border operation interface has been changed to private
* **gui:** The textview widget must operate in the UI thread.
* **gui:** The runtask() method takes two arguments and is called before each task is processed.
* **gui:** Some widget operation functions have been renamed or removed.




# [1.3.0](https://github.com/lc-soft/LCUI/compare/v1.2.0-beta...v1.3.0) (2019-10-07)

### Bug Fixes

* conflict with C++ "operator" keyword ([5a5ba8c](https://github.com/lc-soft/LCUI/commit/5a5ba8c))
* **css:** setting font style parser's style_handler doesn't work ([6869683](https://github.com/lc-soft/LCUI/commit/6869683))
* **display:** memory leak from X11Surface_SetCaptionW() ([484c3d7](https://github.com/lc-soft/LCUI/commit/484c3d7))
* **font:** failed to get bitmap when font_size > 18 ([d6315c5](https://github.com/lc-soft/LCUI/commit/d6315c5))
* **gui:** the default border color of the widget should be transparent ([5164955](https://github.com/lc-soft/LCUI/commit/5164955))
* **gui:** event target selection bug when widget pointer-events is none ([0f26c8b](https://github.com/lc-soft/LCUI/commit/0f26c8b))
* **gui:** TextEdit placeholder are not working ([d827767](https://github.com/lc-soft/LCUI/commit/d827767))
* **gui:** the mousemove event should fire before the mouseout event ([5020b91](https://github.com/lc-soft/LCUI/commit/5020b91))
* **gui:** widget attribute value should always be available ([4b0a2ed](https://github.com/lc-soft/LCUI/commit/4b0a2ed))
* **ime:** composition window position problem ([#36](https://github.com/lc-soft/LCUI/issues/36), [#175](https://github.com/lc-soft/LCUI/issues/175)) ([1107f91](https://github.com/lc-soft/LCUI/commit/1107f91))
* **renderer:** widget content rendering incorrect ([f8b0f8b](https://github.com/lc-soft/LCUI/commit/f8b0f8b))
* **util:** incorrect object type checking in Object_Operate() ([a326e8c](https://github.com/lc-soft/LCUI/commit/a326e8c))
* **util:** ParseUrl() parsing result is incorrect ([3f9450c](https://github.com/lc-soft/LCUI/commit/3f9450c))


### Features

* add LCUI_GetVersion() ([de40c7c](https://github.com/lc-soft/LCUI/commit/de40c7c))
* add rounded border rendering ([#174](https://github.com/lc-soft/LCUI/issues/174)) ([f36d071](https://github.com/lc-soft/LCUI/commit/f36d071))
* add include/LCUI.h file ([#173](https://github.com/lc-soft/LCUI/issues/173)) ([113af6a](https://github.com/lc-soft/LCUI/commit/113af6a))
* **builder:** tag name can be an existing widget type name ([4f6a01c](https://github.com/lc-soft/LCUI/commit/4f6a01c))
* **css:** make CSSFontStyle_* functions public ([568c915](https://github.com/lc-soft/LCUI/commit/568c915))
* **graph:** add LCUI_OverPixel() ([d8075d9](https://github.com/lc-soft/LCUI/commit/d8075d9))
* **gui:** add canvas widget ([e246843](https://github.com/lc-soft/LCUI/commit/e246843))
* **gui:** add Widget_CollectReferences() ([811585b](https://github.com/lc-soft/LCUI/commit/811585b))
* **gui:** add Widget_Each() ([2d7d1ee](https://github.com/lc-soft/LCUI/commit/2d7d1ee))
* **logger:** support setting logging level ([173b92f](https://github.com/lc-soft/LCUI/commit/173b92f))
* **scrollbar:** capture touch and mousewheel events from the container ([f2f9162](https://github.com/lc-soft/LCUI/commit/f2f9162))

# [1.2.0-beta](https://github.com/lc-soft/LCUI/compare/v1.1.0-beta...v1.2.0-beta) (2019-06-17)

### Bug Fixes

* **textlayer:** incorrect text line height calculation ([#169](https://github.com/lc-soft/LCUI/issues/169)) ([a4d2f0a](https://github.com/lc-soft/LCUI/commit/a4d2f0a))

### Features

* **gui:** add default methods for widget prototype ([e68e8e5](https://github.com/lc-soft/LCUI/commit/e68e8e5))
* **gui:** add TextEdit_BindProperty() ([c9d8ded](https://github.com/lc-soft/LCUI/commit/c9d8ded))
* **gui:** add Widget_BindProperty() ([0d3fb68](https://github.com/lc-soft/LCUI/commit/0d3fb68))
* **gui:** add Widget_SetStyleString() ([5163363](https://github.com/lc-soft/LCUI/commit/5163363))
* **gui:** add Widget_SetText() ([02714bf](https://github.com/lc-soft/LCUI/commit/02714bf))
* **textedit:** add TextEdit_GetProperty() ([512706e](https://github.com/lc-soft/LCUI/commit/512706e))
* **util:** add object wrapper and operation set ([241c652](https://github.com/lc-soft/LCUI/commit/241c652))
* **util:** add Object_ToString() ([13465c7](https://github.com/lc-soft/LCUI/commit/13465c7))

# [1.1.0-beta](https://github.com/lc-soft/LCUI/compare/v1.0.0-beta.4...v1.1.0-beta) (2019-03-10)


### Bug Fixes

* **builder:** widget should be initialized before appending ([f12e00b](https://github.com/lc-soft/LCUI/commit/f12e00b))
* **gui:** event target should be initialized ([ebbd128](https://github.com/lc-soft/LCUI/commit/ebbd128))
* **gui:** focus event and "focusable" property parsing bug ([d9a39a5](https://github.com/lc-soft/LCUI/commit/d9a39a5))
* **gui:** mouse events should be ignored when pointer-events is none ([fc54a64](https://github.com/lc-soft/LCUI/commit/fc54a64))
* **gui:** TextEdit should reset the caret position after blur ([8de4e71](https://github.com/lc-soft/LCUI/commit/8de4e71))
* **gui:** TextEdit_SetTextW() does not clear the previous content ([51eef68](https://github.com/lc-soft/LCUI/commit/51eef68))
* **gui:** the button height is different from the textedit ([5a918da](https://github.com/lc-soft/LCUI/commit/5a918da))
* **gui:** widget width with margin is incorrectly calculated ([#167](https://github.com/lc-soft/LCUI/issues/167)) ([d14023e](https://github.com/lc-soft/LCUI/commit/d14023e))
* **thread:** LCUIThread_Join() has not waited for the thread to exit ([4ddb833](https://github.com/lc-soft/LCUI/commit/4ddb833))
* **util:** decoded string length should not include terminator ([93f3d77](https://github.com/lc-soft/LCUI/commit/93f3d77))
* **worker:** task queue should not be blocked when running a task ([2b41f54](https://github.com/lc-soft/LCUI/commit/2b41f54))


### Features

* **gui:** add rule for allow widget updates only when visible ([93049c4](https://github.com/lc-soft/LCUI/commit/93049c4))
* **gui:** add rule for cache children style ([50cc6b2](https://github.com/lc-soft/LCUI/commit/50cc6b2))
* **gui:** add rule for first update visible children ([4cd6fdc](https://github.com/lc-soft/LCUI/commit/4cd6fdc))
* **gui:** add rule for limit the number of children rendered ([ec5e0d6](https://github.com/lc-soft/LCUI/commit/ec5e0d6))
* **gui:** add support for custom widget update rules ([bdd1d1c](https://github.com/lc-soft/LCUI/commit/bdd1d1c))
* **gui:** add widget border helper ([e8d52df](https://github.com/lc-soft/LCUI/commit/e8d52df))
* **gui:** add Widget_GetClosest() ([e740138](https://github.com/lc-soft/LCUI/commit/e740138))
* **gui:** add Widget_SetHashList() ([f466dee](https://github.com/lc-soft/LCUI/commit/f466dee))
* **gui:** auto assign an id to the event name ([fee31b0](https://github.com/lc-soft/LCUI/commit/fee31b0))
* **gui:** Widget_SetAttribute() will call proto->setattr() ([19992d1](https://github.com/lc-soft/LCUI/commit/19992d1))
* add LCUI_Profile for profiling performance ([f715a08](https://github.com/lc-soft/LCUI/commit/f715a08))
* **textview:** add TextView_SetColor() ([3a7b53f](https://github.com/lc-soft/LCUI/commit/3a7b53f))
* **util:** add return value for LCUIRect_ValidateArea() ([2fa6cf3](https://github.com/lc-soft/LCUI/commit/2fa6cf3))
* **util:** add strhash() ([ebf843f](https://github.com/lc-soft/LCUI/commit/ebf843f))
* **util:** add strpool ([c843df1](https://github.com/lc-soft/LCUI/commit/c843df1))
* **util:** add strreplace() ([409395d](https://github.com/lc-soft/LCUI/commit/409395d))
* **util:** default using OutputDebugString() to output log on Windows ([79ec21b](https://github.com/lc-soft/LCUI/commit/79ec21b))


### Performance Improvements

* **css:** change the stylesheet store struct to reduce memory usage ([9cc2957](https://github.com/lc-soft/LCUI/commit/9cc2957))
* **css:** reduce duplicate memory alloc for DictType ([01fd3f6](https://github.com/lc-soft/LCUI/commit/01fd3f6))
* **gui:** improve widget destruction performance ([87aff87](https://github.com/lc-soft/LCUI/commit/87aff87))
* **gui:** improve Widget_Empty() performance ([1654963](https://github.com/lc-soft/LCUI/commit/1654963))
* **gui:** reduce memory usage for widget style sheets ([3790fbd](https://github.com/lc-soft/LCUI/commit/3790fbd))
* **gui:** remove the mutex in the textview widget ([6340d60](https://github.com/lc-soft/LCUI/commit/6340d60))
* **gui:** save classes and status with strlist ([f611936](https://github.com/lc-soft/LCUI/commit/f611936))
* **gui:** update the sort method of the widget list ([e9dbcea](https://github.com/lc-soft/LCUI/commit/e9dbcea))
* **gui:** use LCUI_STYPE_INT instead of LCUI_STYPE_VALUE ([23090eb](https://github.com/lc-soft/LCUI/commit/23090eb))
* **renderer:** improve the selection of the rendering target ([1df08b6](https://github.com/lc-soft/LCUI/commit/1df08b6))



# [1.0.0-beta.4](https://github.com/lc-soft/LCUI/compare/v1.0.0-beta.3...v1.0.0-beta.4) (2018-11-19)

### Bug Fixes

* **dirent:** LCUI_ReadDirW() UNINITIALIZED READ ([dd610ad](https://github.com/lc-soft/LCUI/commit/dd610ad))
* **display:** the renderer pauses when resizing the window ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([8ad667a](https://github.com/lc-soft/LCUI/commit/8ad667a))
* **display:** too many duplicate dirty rectangles in one frame ([b5d9040](https://github.com/lc-soft/LCUI/commit/b5d9040))
* **font:** failed to set default font after loading font file ([9b4c005](https://github.com/lc-soft/LCUI/commit/9b4c005))
* **font:** font number is not reset after module reinitialization ([d75b4ca](https://github.com/lc-soft/LCUI/commit/d75b4ca))
* **gui:** layout should be updated after widget display role is changed ([96c1cef](https://github.com/lc-soft/LCUI/commit/96c1cef))
* **gui:** should clear trash widget when module destroyed ([1ad6be1](https://github.com/lc-soft/LCUI/commit/1ad6be1))
* **gui:** the block element default width should be 100% ([adcb9e7](https://github.com/lc-soft/LCUI/commit/adcb9e7))
* **gui:** the node should unlinked before destroying widget ([3249490](https://github.com/lc-soft/LCUI/commit/3249490))
* **gui:** the unlink event is triggered repeatedly ([89faa5d](https://github.com/lc-soft/LCUI/commit/89faa5d))
* **gui:** widget background image is not rendered ([e76f3f9](https://github.com/lc-soft/LCUI/commit/e76f3f9))
* **gui:** Widget_AutoSize() should not change the static width or height ([8bee9c2](https://github.com/lc-soft/LCUI/commit/8bee9c2))
* **ime:** non-qwerty keyboard layouts broken ([#147](https://github.com/lc-soft/LCUI/issues/147)) ([4b1f050](https://github.com/lc-soft/LCUI/commit/4b1f050))
* **layout:** "left: auto" is calculated as "left: 0" ([f4990da](https://github.com/lc-soft/LCUI/commit/f4990da))
* **platform:** linux framebuffer driver has not been destroyed ([#157](https://github.com/lc-soft/LCUI/issues/157)) ([87f79f0](https://github.com/lc-soft/LCUI/commit/87f79f0))
* **renderer:** problem when widget has transparency ([#160](https://github.com/lc-soft/LCUI/issues/160)) ([d13b554](https://github.com/lc-soft/LCUI/commit/d13b554))
* **renderer:** transparent widgets are rendered incorrectly ([6668165](https://github.com/lc-soft/LCUI/commit/6668165))
* **textlayer:** chinese text line break error ([1e5a262](https://github.com/lc-soft/LCUI/commit/1e5a262))
* **textlayer:** isalpha() assertion failed ([cd39a46](https://github.com/lc-soft/LCUI/commit/cd39a46))
* **textview:** no effect after changing the "content" property ([d1ab50d](https://github.com/lc-soft/LCUI/commit/d1ab50d))
* **textview:** size not updated after setting text ([31e0582](https://github.com/lc-soft/LCUI/commit/31e0582))
* **util:** incorrect value of LCUI_DirEntry::name ([367febe](https://github.com/lc-soft/LCUI/commit/367febe))
* **worker:** worker thread memory access violation ([5a7e2c0](https://github.com/lc-soft/LCUI/commit/5a7e2c0))
* linker errors when jpeg and png are not supported ([2bbabe1](https://github.com/lc-soft/LCUI/commit/2bbabe1))
* PACKAGE_VERSION is not defined ([70660de](https://github.com/lc-soft/LCUI/commit/70660de))
* the mouse cursor should be hidden when the window system exists ([acf9454](https://github.com/lc-soft/LCUI/commit/acf9454))
* the return value of LCUI_GetAppId() is incorrect ([46ec607](https://github.com/lc-soft/LCUI/commit/46ec607))
* "linux/input.h" file not found on FreeBSD ([7d95f3f](https://github.com/lc-soft/LCUI/commit/7d95f3f))
* undefined ENODATA on FreeBSD ([2cd0b27](https://github.com/lc-soft/LCUI/commit/2cd0b27))
* unresolved external symbol _Graph_IsValid ([1328a2d](https://github.com/lc-soft/LCUI/commit/1328a2d))

### Features

* **builder:** `<resource>` support load xml file ([d5e162c](https://github.com/lc-soft/LCUI/commit/d5e162c))
* **display:** auto disable window maximize button ([#164](https://github.com/lc-soft/LCUI/issues/164)) ([82fad1b](https://github.com/lc-soft/LCUI/commit/82fad1b))
* **event:** add ctrl_key and shift_key to LCUI_KeyboardEvent ([6f5f17c](https://github.com/lc-soft/LCUI/commit/6f5f17c))
* **gui:** add Widget_SetOpacity() ([e6ad163](https://github.com/lc-soft/LCUI/commit/e6ad163))
* **gui:** add Widget_SetVisibility() ([145d9ee](https://github.com/lc-soft/LCUI/commit/145d9ee))
* **gui:** add Widget_SetVisible() and Widget_SetHidden() ([2f581b8](https://github.com/lc-soft/LCUI/commit/2f581b8))
* **gui:** LCUIWidget_ClearTrash() will return count ([6262f20](https://github.com/lc-soft/LCUI/commit/6262f20))
* **linux:** add mouse wheel event handing for x11 ([#54](https://github.com/lc-soft/LCUI/issues/54)) ([1061592](https://github.com/lc-soft/LCUI/commit/1061592))
* **linux:** use fontconfig to locate fonts ([2932246](https://github.com/lc-soft/LCUI/commit/2932246))
* **textview:** refresh all textview after loading new font ([3fcfa99](https://github.com/lc-soft/LCUI/commit/3fcfa99))
* **util:** add OpenUri() ([ec20c99](https://github.com/lc-soft/LCUI/commit/ec20c99))
* make LCUI_MAX_FRAMES_PER_SEC public ([6fa2995](https://github.com/lc-soft/LCUI/commit/6fa2995))
* add LCUI_MAX_FRAME_MSEC macro ([7abc901](https://github.com/lc-soft/LCUI/commit/7abc901))

### Performance Improvements

* **charset:** improve utf-8 and unicode string convert ([1efd856](https://github.com/lc-soft/LCUI/commit/1efd856))
* **graph:** improve image scaling quality (issue [#39](https://github.com/lc-soft/LCUI/issues/39)) ([bbfb9af](https://github.com/lc-soft/LCUI/commit/bbfb9af))

## 1.0.0 Beta 3 (2018-05-08)

### Bug Fixes

* **font:** segmentation fault in DeleteFont() ([5465c6b](https://github.com/lc-soft/LCUI/commit/5465c6b))
* **timer:** timer thread may quit after it is created ([0b01f88](https://github.com/lc-soft/LCUI/commit/0b01f88))
* **util:** 'struct dirent' has no member named 'd_reclen' ([#141](https://github.com/lc-soft/LCUI/issues/141)) ([0416c42](https://github.com/lc-soft/LCUI/commit/0416c42))

### Features

* **platform:** add linux framebuffer driver ([6015838](https://github.com/lc-soft/LCUI/commit/6015838))
* **platform:** add linux keyboard driver ([ad3348e](https://github.com/lc-soft/LCUI/commit/ad3348e))
* **platform:** add linux mouse driver ([0ff7b70](https://github.com/lc-soft/LCUI/commit/0ff7b70))

## 1.0.0 Beta 2 (2018-03-18)

### Bug Fixes

* some memory leak bugs ([#135](https://github.com/lc-soft/LCUI/issues/135)) ([9995b23](https://github.com/lc-soft/LCUI/commit/9995b23))
* **gui:** widget auto size computation bug ([095f4b8](https://github.com/lc-soft/LCUI/commit/095f4b8))
* **renderer:** incorrect widget content rectangle computation ([#122](https://github.com/lc-soft/LCUI/issues/122),[#123](https://github.com/lc-soft/LCUI/issues/123)) ([0f81863](https://github.com/lc-soft/LCUI/commit/0f81863))
* **renderer:** widget content overflow ([#144](https://github.com/lc-soft/LCUI/issues/144)) ([2a923a6](https://github.com/lc-soft/LCUI/commit/2a923a6))
* **thread:** memory leak bug when running test ([402bc03](https://github.com/lc-soft/LCUI/commit/402bc03))
* **widget:** scrollbar did not cancel the event bubbling ([#145](https://github.com/lc-soft/LCUI/issues/145)) ([7dd60ac](https://github.com/lc-soft/LCUI/commit/7dd60ac))
* **widget:** Scrollbar_BindBox() not working after scrollbar initialize ([196f47c](https://github.com/lc-soft/LCUI/commit/196f47c))
* **widget:** Widget_GetOffset() result does not include padding spacing ([0a893f4](https://github.com/lc-soft/LCUI/commit/0a893f4))
* **widget-event:** the touch point coordinate are not been converted ([a40eda2](https://github.com/lc-soft/LCUI/commit/a40eda2))
* **worker:** worker did not enter the blocked state ([#134](https://github.com/lc-soft/LCUI/issues/134)) ([8dae96f](https://github.com/lc-soft/LCUI/commit/8dae96f))

### Features

* **css:** add parsing support for "border-left: 0;" ([30de5b8](https://github.com/lc-soft/LCUI/commit/30de5b8))
* **timer:** add LCUITimer_SetTimeout() and LCUITimer_SetInterval() ([f032f6f](https://github.com/lc-soft/LCUI/commit/f032f6f))
* **widget-event:** add "link" event, rename "remove" event to "unlink" ([8c1d105](https://github.com/lc-soft/LCUI/commit/8c1d105))

### Performance Improvements

* **widget:** rename scrollbar widget attributes ([d397914](https://github.com/lc-soft/LCUI/commit/d397914))

## 1.0.0 Beta (2018-02-07)

### Features

- **font:** add support for font-weight and font-style selection
- **textlayer:** add `[bgcolor]` tag support (#110)
- **textlayer:** add `[i]` tag support (#115)
- **textlayer:** add `[b]` tag support (#112)
- **textlayer:** using FreeType to get space width
- **widget:** add content trimming support for textview (#97)
- **widget:** add anchor widget (#105)
- **widget:** add word-break property support for textview (#71)
- **widget:** add `WTT_RESIZE_WITH_SURFACE` task for sync surface size (#104)
- **widget:** add Widget_UnbindEventByHandlerId() function
- **css:** add css font-face rule parser (#95)
- **css:** add relative path support for url() parser (#98)
- **css:** add css property parser for parsing justify-content (#117)
- **css:** add support to parse "border: 0"
- **css:** add support to parse "line-height: 1"
- **layout:** add simple support for flex display and justify-content (#117)
- **bulder:** add short name `<w>` for `<widget>` (#103)
- **core:** add LCUIWorker for async task support (#106)

### Bug fixes

- **textlayer:** text vertical position is not center (#100)
- **widget:** textview height computation error (#111)
- **css:** stylesheet selection bug (#113)
- **graphic:** segmentation fault bug in Graph_FillRectARGB() (#110)
- **linux:** set "--enable-video-ouput=no" will make the linker output errors
- **renderer:** widget border rendering is incomplete when position is non-integer (#108)

### Improvements

- **layout:** improve widget size computation
- **font:** improve font data storage and operation method
- **mainloop:** set run all tasks in one frame
- **mainloop:** set the widget tree to update least once per frame

You can view [this pull request](https://github.com/lc-soft/LCUI/pull/120). for detailed code changes.

## CHANGES BETWEEN 1.0 alpha and 1.0 alpha2

- Adjust the code structure
- Improved TextView widget
- Improved caret positioning and text rendering for TextEdit widget
- Improved widget box-shadow rendering
- Add drivers for UWP Application, [here](https://github.com/lc-soft/LCUI/blob/develop/build/windows/LCUIApp/App.cpp) is example
- Improved support for high-resolution screen
  - Add dp and sp unit, Similar to dp and sp unit in Android
  - Add support scaling, It is possible to set the appropriate zoom ratio according to the screen pixel density
- Improve automation build and test
  - Add unit testing
  - Add code coverage testing
  - Enabled the [valgrind](http://valgrind.org/) memory check tool
- Fixed all memory leak BUGs and memory access errors
- Fixed some widget layout bugs
- Fixed jpeg reader sometimes fail to read data
- Fixed other known bugs

You can view [this pull request](https://github.com/lc-soft/LCUI/pull/83). for detailed code changes.

## CHANGES BETWEEN 0.15.0 and 1.0 alpha

- Improved bmp, jpeg, png file processing, add image reader
- Added Scrollbar widget
- Improved Button widget
- Improved TextView widget
- Improved TextEdit widget
- Improved widget types, events, layouts and styles processing
- Imrpoved UI rendering performance
- Added touch screen support
- Added XML and CSS support
- Removed Linux Framebuffer support
- Added Linux XWindow support
- Added simple support for Universal Windows Platform (UWP)
- Imrpoved Windows support
- Added some tutorials (only Chinese version)
- Fixed some bugs

## CHANGES BETWEEN 0.14.0 and 0.15.0

- Limit the graphical render maximum frames per second to 100, for reduce CPU usage.
- Modify event processing module, add an event disconnection functions, partial function has been modified.
- Update timer function modules, fix known BUG.
- Update TextBox and Label GUI Widget, optimized text rendering speed, fix known BUG.
- Add text auto-wrap function, if edit text in TextBox, may be problems when enabled auto-wrap function.
- Fixed: multi-threaded rendering text causes text bitmap abnormal, program crash and other problems, now the text rendering tasks has been transferred to the main thread performed separately.
- Fixed: known BUG in GUI widgets stacking order processing function.
- Fixed: all messages are not all processed when each processing GUI Widget messages. 

This updates mainly aimed author's LCUI 2D Fighting Game, since the LCUI 2D Fighting Game is mainly on the windows platform for development and testing, therefore, although some function modules on the windows platform support is good, but is not guaranteed can be normal support  GNU / Linux platform.


## CHANGES BETWEEN 0.13.0 and 0.14.0

- Improved support for windows platform, optimize the graphics output,
 improving the keyboard and mouse input support.
- You can set the video output mode, but valid only in the windows platform.
- Solve some of the modules thread-safety issues.
- Optimize GUI-Widget graphics resources memory usage.
- Improved GUI-Widget message processing.
- Improved GUI-Widget display order processing.
- Adjusted GUI-Widget click event and dragging event handling.
- GUI-Widget support by setting z-index value to adjust the display order.
- Add input method framework and default input method, currently only supports
 input English letters, numbers and symbols.
- For built-in bitmap font file (for example: simsun.ttc), is now able to
 normally display the font bitmap.
- Solve other BUGS.


## CHANGES BETWEEN 0.12.6 and 0.13.0

- Most of modules function naming style has been changed.
- Solve Multi-window display ordering issues.
- Improve GUI Widget processing module.
- Add event loop, improve GUI Widget event mechanism.
- Improve thread management module, Package linux and win32 thread 
  management and mutex related functions.
- Add win32 graphics output module , due to the limited level, 
  currently could only output graphics in the client area of the 
  window, needs to be improved.
- Add font management module, improve font bitmap processing module,
  optimize the loading speed of bitmap fonts, reduce the memory overhead.
- Add GraphLayer module, used to manage each widget graphics layer,
  LCUI video output mainly generated by the module processing.
- Add placeholder function for the TextBox Widget.
- Add MessageBox.
- Solve other BUGS.

Now, we are achieve the LCUI running on win32, you can use 
Visual Studio 2012 to directly compile this project source code, 
but exist some problems that need to be improved.


## CHANGES BETWEEN 0.12.5 and 0.12.6

- LCUI class renamed to LCUIApp, some widget C++ class changed to 
  inherited from the LCUIWidget class, cancel previous nested class.
- Re-planning project source code, and prepare for the future of 
  cross-platform.
- Add new widget: TextBox and ScrollBar, the TextBox also only have 
  the basic functions, pending to continue to improve.
- Add TextLayer module for handling text-bitmap drawing.
- The default style of window has been modified.
- Add timer function, ActvieBox widget animation each frame update 
  has been changed to use the timer.
- widget can get focus, you can connect FocusIn and FocusOut event 
  for response to widget get/lose focus.
- Keyboard input event mechanism has been modified, when keyboard 
  input, keyboard event default sent to the Focus Widget.
- Re-planning widget attributes, the widget update processing function 
  has been modified.
- Add new widget attributes: padding, you can adjust the padding 
  of the widget.
- Add new widget attributes: dock, you can set Widget's dock type.
- Add STATIC positioning type for widget, this positioning type of 
  widget will according to display order Auto-Arrange.
- Widget size support expressed as percentage, When the container 
  size has been changed, the widget's size will also change.


## CHANGES BETWEEN 0.12.4 and 0.12.5

- Add C + + class to some function modules, please see the 
  helloworld2.cpp file for more details.
- Add ActiveBox Widget, it can achieve the continuity of a simple
  multi-frame image switching display.
- Improved screen graphics refresh method.
- Solve the problem of disappear  after dragging the child window,  
  the reason is that the sub window position calculation is error.
- Optimized Alpha blending processing, efficiency has improved.
- Solve some of the problems of graphics processing.
- LCUI_Graph support the "quote" function, you can quote graph 
  of a specified area in an graph.
- Solve some of the problems of the PictureBox Widget.
- Improved the nested multi Widget graphic display processing.
- Improved the area refresh processing of widget moving.
- Some settings support environment variable setting.
- Solve the thread is blocked waiting for unlock the rwlock 
  problem, the problem is in the LCUI exit.


## CHANGES BETWEEN 0.12.3 and 0.12.4

- corrected the graphical display problems of the widget nesting. 
- the graphical display method has been changed, in theory,
  graphics refresh rate will be enhanced.
- some functions and data structures has been modified.
- add the RadioButton widget and CheckBox widget.
- the event mechanism has been modified, add mouse move / click 
  event and widget drag event.
- add graphics data read-write lock function to protect data, 
  prevent multiple threads simultaneously read / write.
- the header file has been modified, use LCUI library function, 
  you may need to include the appropriate header file.
- the graphical output function has been modified, not only 
  support 24 and 32-bit color, but also support on other color 
  monitor to display graphics.
- some source file has been merged.
- configure.ac and the README file has been modified.

