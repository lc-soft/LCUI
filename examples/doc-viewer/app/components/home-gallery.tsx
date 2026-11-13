import {
  Button,
  FunctionWidget,
  RouterLink,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Text,
} from "@lcui/react";

type Locale = "zh-CN" | "en";

const heroDesc: Record<Locale, string> = {
  "zh-CN": "用于开发图形用户界面的 C 语言库。",
  en: "The C library for building user interfaces.",
};

const ctaLabel: Record<Locale, { start: string; widgets: string }> = {
  "zh-CN": { start: "快速开始", widgets: "浏览部件" },
  en: { start: "Get started", widgets: "Browse widgets" },
};

const copyByLocale = {
  en: {
    controls_button: "Button",
    controls_secondary: "Secondary",
    controls_outline: "Outline",
    controls_name: "Name",
    controls_message: "Message",
    controls_badge: "Badge",
    controls_alert_dialog: "Alert Dialog",
    controls_alert_title: "Allow accessory to connect?",
    controls_alert_desc:
      "Do you want to allow the USB accessory to connect to this device and your data?",
    controls_deny: "Don't allow",
    controls_allow: "Allow",
    controls_button_group: "Button Group",
    controls_quick_actions: "Quick Actions",
    controls_mute: "Mute Conversation",
    controls_mark_read: "Mark as Read",
    controls_block: "Block User",
    controls_delete: "Delete Conversation",
    attachments_title: "Attachments",
    attachments_description: "Upload, review, and manage your files.",
    attachments_ready: "Ready to upload",
    attachments_uploading: "Uploading - 64%",
    attachments_processing: "Processing document",
    attachments_failed: "Upload failed. Try again.",
    attachments_uploaded: "Uploaded - 1.8 MB",
    savings_title: "Savings Targets",
    savings_description:
      "Active milestones for 2024 across your portfolio. Monitor how close you are to each savings goal.",
    savings_retirement: "RETIREMENT",
    savings_retirement_value: "$420,000",
    savings_retirement_achieved: "65% achieved",
    savings_retirement_amount: "$273,000",
    savings_real_estate: "REAL ESTATE",
    savings_real_estate_value: "$85,000",
    savings_real_estate_achieved: "32% achieved",
    savings_real_estate_amount: "$27,200",
    savings_warning: "You have not met your targets for this year.",
    claim_label: "Claimable Balance",
    claim_balance: "$1,211.29",
    claim_pending: "Pending Setup",
    claim_net_label: "Net Royalties",
    claim_net: "$1,248.75",
    claim_fee_label: "Processing Fee",
    claim_fee: "-$37.46",
    claim_total_label: "Total Ready to Claim",
    claim_total: "$1,211.29 USD",
    claim_description:
      "Once your bank is connected, balances over $10 are automatically eligible for monthly distribution on the 15th of each month.",
    deployment_title: "Deployment Status",
    deployment_live: "Live",
    deployment_description: "Production rollout for the documentation site.",
    deployment_environment: "production",
    deployment_percent: "82%",
    deployment_build: "Build & test",
    deployment_build_desc: "Windows, Linux and macOS passed.",
    deployment_duration1: "2m 14s",
    deployment_database: "Database migration",
    deployment_database_desc: "Schema changes applied successfully.",
    deployment_duration2: "38s",
    deployment_edge: "Edge rollout",
    deployment_edge_desc: "Shifting traffic across regions.",
    deployment_warning_title: "One region is warming up",
    deployment_warning_desc: "Traffic is still being shifted in eu-west.",
    deployment_view_logs: "View logs",
    deployment_rollback: "Rollback release",
    deployment_copy_hash: "Copy commit hash",
    deployment_redeploy: "Redeploy",
    milestone_title: "Set a new milestone",
    milestone_description:
      "Define your financial target and we'll help you pace your savings.",
    milestone_goal: "Goal Name",
    milestone_goal_placeholder: "e.g. New Car",
    milestone_target_amount: "Target Amount",
    milestone_target_placeholder: "$15,000",
    milestone_target_date: "Target Date",
    milestone_date_placeholder: "Dec 2025",
    milestone_create: "Create Goal",
    milestone_cancel: "Cancel",
    subscription_title: "Subscription & Billing",
    subscription_description:
      "Common questions about your account, plans, payments and cancellations.",
    subscription_q1: "What subscription plans do you offer?",
    subscription_a1:
      "We offer three subscription tiers: Starter ($9/month), Professional ($29/month), and Enterprise ($99/month). Each plan includes increasing storage limits, API access, priority support, and team collaboration features.",
    subscription_q2: "How does billing work?",
    subscription_a2:
      "Billing occurs automatically at the start of each billing cycle. We accept all major credit cards, PayPal, and ACH transfers for enterprise customers.",
    subscription_q3: "How do I cancel my subscription?",
    subscription_a3:
      "You can cancel your subscription anytime from your account settings. There are no cancellation fees or penalties.",
    subscription_q4: "Can I change plans later?",
    subscription_a4:
      "Yes. Upgrades take effect immediately and downgrades apply at the end of the current billing period.",
    payout_title: "Payout Threshold",
    payout_description:
      "Set the minimum balance required before a payout is triggered.",
    payout_currency_label: "Preferred Currency",
    payout_currency: "USD — United States Dollar",
    payout_currency_value: "usd",
    payout_usd: "USD — United States Dollar",
    payout_cny: "CNY — Chinese Yuan",
    payout_jpy: "JPY — Japanese Yen",
    payout_eur: "EUR — Euro",
    payout_min_label: "Minimum Payout Amount",
    payout_amount: "$2500.00",
    payout_min: "$50 (MIN)",
    payout_max: "$10,000 (MAX)",
    payout_notes: "Notes",
    payout_note_placeholder: "Add a note for this payout",
    payout_save: "Save Threshold",
    account_title: "Account Access",
    account_description: "Update your credentials or re-authenticate.",
    account_email: "Email Address",
    account_password: "Current Password",
    account_forgot: "FORGOT?",
    account_update: "Update Security",
    account_danger: "Danger Zone",
    account_danger_desc: "Archive account and remove…",
    distribute_title: "Distribute Track",
    distribute_description:
      "Upload your first master to start reaching listeners on Spotify, Apple Music, and more.",
    distribute_create: "Create Release",
    notifications_title: "Notifications",
    notifications_description:
      "Choose which email and push alerts you want to receive.",
    notifications_transaction: "Transaction alerts",
    notifications_transaction_desc: "Deposits, withdrawals, and transfers.",
    notifications_security: "Security alerts",
    notifications_security_desc: "Login attempts and account changes.",
    notifications_milestones: "Goal milestones",
    notifications_milestones_desc: "Updates at 25%, 50%, 75%, and 100%.",
    notifications_market: "Market updates",
    notifications_market_desc: "Daily portfolio summary and price alerts.",
    notifications_save: "Save Preferences",
    shortcuts_breadcrumb: "/Home/Profile/Statements/Documents/Payments",
    shortcuts_change_limit: "Change transfer limit",
    shortcuts_change_limit_desc:
      "Adjust how much you can send from your balance.",
    shortcuts_scheduled: "Scheduled transfers",
    shortcuts_scheduled_desc: "Set up a transfer to send at a later date.",
    shortcuts_recurring: "Recurring card payments",
    shortcuts_recurring_desc: "Manage your repeated card transactions.",
  },
  "zh-CN": {
    controls_button: "按钮",
    controls_secondary: "次要",
    controls_outline: "描边",
    controls_name: "姓名",
    controls_message: "消息",
    controls_badge: "徽章",
    controls_alert_dialog: "警告对话框",
    controls_alert_title: "允许配件连接吗？",
    controls_alert_desc: "是否允许 USB 配件连接此设备并访问你的数据？",
    controls_deny: "不允许",
    controls_allow: "允许",
    controls_button_group: "按钮组",
    controls_quick_actions: "快捷操作",
    controls_mute: "静音会话",
    controls_mark_read: "标为已读",
    controls_block: "屏蔽用户",
    controls_delete: "删除会话",
    attachments_title: "附件",
    attachments_description: "上传、查看并管理你的文件。",
    attachments_ready: "等待上传",
    attachments_uploading: "上传中 - 64%",
    attachments_processing: "正在处理文档",
    attachments_failed: "上传失败，请重试。",
    attachments_uploaded: "已上传 - 1.8 MB",
    savings_title: "储蓄目标",
    savings_description:
      "2024 年的活跃里程碑，覆盖你的整个投资组合。随时了解你离每个储蓄目标还有多远。",
    savings_retirement: "退休",
    savings_retirement_value: "￥420,000",
    savings_retirement_achieved: "已完成 65%",
    savings_retirement_amount: "￥273,000",
    savings_real_estate: "房地产",
    savings_real_estate_value: "￥85,000",
    savings_real_estate_achieved: "已完成 32%",
    savings_real_estate_amount: "￥27,200",
    savings_warning: "你尚未达成本年度目标。",
    claim_label: "可提取余额",
    claim_balance: "￥1,211.29",
    claim_pending: "待设置",
    claim_net_label: "净版税",
    claim_net: "￥1,248.75",
    claim_fee_label: "处理费",
    claim_fee: "-￥37.46",
    claim_total_label: "可提取总额",
    claim_total: "￥1,211.29",
    claim_description:
      "绑定银行账户后，余额超过 ￥10 将自动获得每月 15 日发放的资格。",
    deployment_title: "部署状态",
    deployment_live: "在线",
    deployment_description: "文档站点的生产环境发布。",
    deployment_environment: "生产环境",
    deployment_percent: "82%",
    deployment_build: "构建与测试",
    deployment_build_desc: "Windows、Linux 和 macOS 均已通过。",
    deployment_duration1: "2分14秒",
    deployment_database: "数据库迁移",
    deployment_database_desc: "架构变更已成功应用。",
    deployment_duration2: "38秒",
    deployment_edge: "边缘节点发布",
    deployment_edge_desc: "正在跨区域切换流量。",
    deployment_warning_title: "一个区域正在预热",
    deployment_warning_desc: "eu-west 的流量仍在切换。",
    deployment_view_logs: "查看日志",
    deployment_rollback: "回滚版本",
    deployment_copy_hash: "复制提交哈希",
    deployment_redeploy: "重新部署",
    milestone_title: "设置新里程碑",
    milestone_description: "定义你的财务目标，我们会帮你规划储蓄节奏。",
    milestone_goal: "目标名称",
    milestone_goal_placeholder: "例如：新车",
    milestone_target_amount: "目标金额",
    milestone_target_placeholder: "￥15,000",
    milestone_target_date: "目标日期",
    milestone_date_placeholder: "2025年12月",
    milestone_create: "创建目标",
    milestone_cancel: "取消",
    subscription_title: "订阅与账单",
    subscription_description: "关于账户、套餐、付款和取消的常见问题。",
    subscription_q1: "你们提供哪些订阅套餐？",
    subscription_a1:
      "我们提供三档订阅：Starter（￥9/月）、Professional（￥29/月）和 Enterprise（￥99/月）。每个套餐都包含更高的存储上限、API 访问、优先支持和团队协作功能。",
    subscription_q2: "账单如何计算？",
    subscription_a2:
      "账单会在每个计费周期开始时自动扣款。我们支持所有主流信用卡、PayPal，以及企业客户的 ACH 转账。",
    subscription_q3: "如何取消订阅？",
    subscription_a3: "你可以随时在账户设置中取消订阅。没有取消费或违约金。",
    subscription_q4: "之后可以更改套餐吗？",
    subscription_a4: "可以。升级会立即生效，降级会在当前计费周期结束时生效。",
    payout_title: "提现阈值",
    payout_description: "设置触发提现所需的最低余额。",
    payout_currency_label: "首选货币",
    payout_currency: "CNY — 人民币",
    payout_currency_value: "cny",
    payout_usd: "USD — 美元",
    payout_cny: "CNY — 人民币",
    payout_jpy: "JPY — 日元",
    payout_eur: "EUR — 欧元",
    payout_min_label: "最低提现金额",
    payout_amount: "￥2500.00",
    payout_min: "￥50（最低）",
    payout_max: "￥10,000（最高）",
    payout_notes: "备注",
    payout_note_placeholder: "为此提现添加备注",
    payout_save: "保存阈值",
    account_title: "账户访问",
    account_description: "更新你的凭据或重新进行身份验证。",
    account_email: "邮箱地址",
    account_password: "当前密码",
    account_forgot: "忘记密码？",
    account_update: "更新安全设置",
    account_danger: "危险区域",
    account_danger_desc: "归档账户并移除…",
    distribute_title: "分发曲目",
    distribute_description:
      "上传你的第一首母带，开始在 Spotify、Apple Music 等平台触达听众。",
    distribute_create: "创建发行",
    notifications_title: "通知",
    notifications_description: "选择你希望接收的邮件和推送提醒。",
    notifications_transaction: "交易提醒",
    notifications_transaction_desc: "存款、提现和转账。",
    notifications_security: "安全提醒",
    notifications_security_desc: "登录尝试和账户变更。",
    notifications_milestones: "目标里程碑",
    notifications_milestones_desc: "在 25%、50%、75% 和 100% 时更新。",
    notifications_market: "市场动态",
    notifications_market_desc: "每日投资组合摘要和价格提醒。",
    notifications_save: "保存偏好",
    shortcuts_breadcrumb: "/首页/个人资料/对账单/文档/付款",
    shortcuts_change_limit: "更改转账限额",
    shortcuts_change_limit_desc: "调整你可以从余额中发送的金额。",
    shortcuts_scheduled: "定时转账",
    shortcuts_scheduled_desc: "设置稍后发送的转账。",
    shortcuts_recurring: "周期卡支付",
    shortcuts_recurring_desc: "管理你的重复卡交易。",
  },
} as const;

const HomeGallery: FunctionWidget = ({ locale }: { locale: Locale }) => {
  const cta = ctaLabel[locale];
  const copy = copyByLocale[locale];

  return (
    <div className="w-full h-full">
      <ScrollArea className="w-full h-full">
        <ScrollAreaContent>
          <div className="home-page">
            <div className="home-inner">
              <div className="home-hero">
                <Text className="home-title">LCUI</Text>
                <Text className="home-desc">{heroDesc[locale]}</Text>
                <div className="home-cta">
                  <RouterLink
                    className="button button-primary"
                    to={`/${locale}/docs/overview/quick-start`}
                  >
                    <Text>{cta.start}</Text>
                  </RouterLink>
                  <RouterLink
                    className="button button-outline"
                    to={`/${locale}/docs/widgets`}
                  >
                    <Text>{cta.widgets}</Text>
                  </RouterLink>
                </div>
              </div>

              <home-grid>
                <card className="home-card" data-column="1">
                  <card-content className="home-card-body home-controls clip-safe">
                    <div className="home-row">
                      <Button variant="primary" size="sm">
                        <Text>{copy.controls_button}</Text>
                        <icon name="arrow-right" />
                      </Button>
                      <Button variant="secondary" size="sm">
                        {copy.controls_secondary}
                      </Button>
                      <Button variant="outline" size="sm">
                        {copy.controls_outline}
                      </Button>
                    </div>
                    <input-group>
                      <textinput
                        className="input-group-input"
                        placeholder={copy.controls_name}
                      />
                      <input-group-addon align="inline-end">
                        <icon name="search" />
                      </input-group-addon>
                    </input-group>
                    <textinput
                      className="home-textarea"
                      multiline="true"
                      placeholder={copy.controls_message}
                    />
                    <div className="home-row home-row-between">
                      <div className="home-row">
                        <badge variant="default">{copy.controls_badge}</badge>
                        <badge variant="secondary">
                          {copy.controls_secondary}
                        </badge>
                      </div>
                      <div className="home-row">
                        <radio-group value="a">
                          <radio-group-item value="a" checked="true" />
                          <radio-group-item value="b" />
                        </radio-group>
                        <checkbox checked="true" />
                      </div>
                    </div>
                    <div className="home-row">
                      <alert-dialog>
                        <alert-dialog-trigger>
                          <Button variant="outline">
                            {copy.controls_alert_dialog}
                          </Button>
                        </alert-dialog-trigger>
                        <alert-dialog-overlay>
                          <alert-dialog-content>
                            <alert-dialog-header>
                              <alert-dialog-title>
                                {copy.controls_alert_title}
                              </alert-dialog-title>
                              <alert-dialog-description>
                                {copy.controls_alert_desc}
                              </alert-dialog-description>
                            </alert-dialog-header>
                            <alert-dialog-footer>
                              <alert-dialog-cancel variant="outline">
                                {copy.controls_deny}
                              </alert-dialog-cancel>
                              <alert-dialog-action variant="primary">
                                {copy.controls_allow}
                              </alert-dialog-action>
                            </alert-dialog-footer>
                          </alert-dialog-content>
                        </alert-dialog-overlay>
                      </alert-dialog>
                      <button-group>
                        <Button variant="outline">
                          {copy.controls_button_group}
                        </Button>
                        <dropdown-menu>
                          <dropdown-menu-trigger>
                            <Button variant="outline" size="icon">
                              <icon name="chevron-down" />
                            </Button>
                          </dropdown-menu-trigger>
                          <dropdown-menu-content align="end">
                            <dropdown-menu-label>
                              <Text>{copy.controls_quick_actions}</Text>
                            </dropdown-menu-label>
                            <dropdown-menu-item>
                              <Text>{copy.controls_mute}</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>{copy.controls_mark_read}</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>{copy.controls_block}</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-separator />
                            <dropdown-menu-item variant="destructive">
                              <Text>{copy.controls_delete}</Text>
                            </dropdown-menu-item>
                          </dropdown-menu-content>
                        </dropdown-menu>
                      </button-group>
                    </div>
                  </card-content>
                </card>

                <card className="home-card" data-column="1">
                  <card-content className="home-card-body home-attachment-card">
                    <card-header>
                      <card-title>{copy.attachments_title}</card-title>
                      <card-description>
                        {copy.attachments_description}
                      </card-description>
                    </card-header>
                    <attachment state="idle">
                      <attachment-media>
                        <icon name="clock" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>selected-file.pdf</attachment-title>
                        <attachment-description>
                          {copy.attachments_ready}
                        </attachment-description>
                      </attachment-content>
                      <attachment-actions>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="dismiss" />
                        </Button>
                      </attachment-actions>
                    </attachment>
                    <attachment state="uploading">
                      <attachment-media>
                        <spinner running="true" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>design-system.zip</attachment-title>
                        <attachment-description>
                          {copy.attachments_uploading}
                        </attachment-description>
                      </attachment-content>
                      <attachment-actions>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="dismiss" />
                        </Button>
                      </attachment-actions>
                    </attachment>
                    <attachment state="processing">
                      <attachment-media>
                        <icon name="document-text" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>market-research.pdf</attachment-title>
                        <attachment-description>
                          {copy.attachments_processing}
                        </attachment-description>
                      </attachment-content>
                      <attachment-actions>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="dismiss" />
                        </Button>
                      </attachment-actions>
                    </attachment>
                    <attachment state="error">
                      <attachment-media>
                        <icon name="document-error" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>
                          financial-model.xlsx
                        </attachment-title>
                        <attachment-description>
                          {copy.attachments_failed}
                        </attachment-description>
                      </attachment-content>
                      <attachment-actions>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="arrow-sync" />
                        </Button>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="dismiss" />
                        </Button>
                      </attachment-actions>
                    </attachment>
                    <attachment state="done">
                      <attachment-media>
                        <icon name="checkmark" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>uploaded-report.pdf</attachment-title>
                        <attachment-description>
                          {copy.attachments_uploaded}
                        </attachment-description>
                      </attachment-content>
                      <attachment-actions>
                        <Button
                          variant="ghost"
                          size="icon-xs"
                          className="attachment-action"
                        >
                          <icon name="dismiss" />
                        </Button>
                      </attachment-actions>
                    </attachment>
                  </card-content>
                </card>

                <card className="home-card" data-column="1">
                  <card-content className="home-card-body home-savings">
                    <card-header>
                      <card-title>{copy.savings_title}</card-title>
                      <card-description>
                        {copy.savings_description}
                      </card-description>
                    </card-header>
                    <div className="home-goal">
                      <Text className="home-goal-label">
                        {copy.savings_retirement}
                      </Text>
                      <Text className="home-goal-value">
                        {copy.savings_retirement_value}
                      </Text>
                      <progress value="65" className="home-goal-progress" />
                      <div className="home-goal-footer">
                        <Text className="home-goal-caption">
                          {copy.savings_retirement_achieved}
                        </Text>
                        <Text className="home-goal-amount">
                          {copy.savings_retirement_amount}
                        </Text>
                      </div>
                    </div>
                    <div className="home-goal">
                      <Text className="home-goal-label">
                        {copy.savings_real_estate}
                      </Text>
                      <Text className="home-goal-value">
                        {copy.savings_real_estate_value}
                      </Text>
                      <progress value="32" className="home-goal-progress" />
                      <div className="home-goal-footer">
                        <Text className="home-goal-caption">
                          {copy.savings_real_estate_achieved}
                        </Text>
                        <Text className="home-goal-amount">
                          {copy.savings_real_estate_amount}
                        </Text>
                      </div>
                    </div>
                    <Text className="home-preview-desc">
                      {copy.savings_warning}
                    </Text>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-deployment">
                    <card-header>
                      <div className="home-row home-row-between">
                        <card-title>{copy.deployment_title}</card-title>
                        <badge variant="outline">
                          <div className="home-deploy-live-dot" />
                          <Text>{copy.deployment_live}</Text>
                        </badge>
                      </div>
                      <card-description>
                        {copy.deployment_description}
                      </card-description>
                    </card-header>
                    <div className="home-deploy-progress">
                      <div className="home-row home-row-between">
                        <Text className="home-deploy-environment">
                          {copy.deployment_environment}
                        </Text>
                        <Text className="home-deploy-percent">
                          {copy.deployment_percent}
                        </Text>
                      </div>
                      <progress value="82" />
                    </div>
                    <div className="home-deploy-steps">
                      <item>
                        <item-media variant="icon">
                          <icon name="checkmark" />
                        </item-media>
                        <item-content>
                          <item-title>{copy.deployment_build}</item-title>
                          <item-description>
                            {copy.deployment_build_desc}
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">
                            {copy.deployment_duration1}
                          </Text>
                        </item-actions>
                      </item>
                      <item>
                        <item-media variant="icon">
                          <icon name="checkmark" />
                        </item-media>
                        <item-content>
                          <item-title>{copy.deployment_database}</item-title>
                          <item-description>
                            {copy.deployment_database_desc}
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">
                            {copy.deployment_duration2}
                          </Text>
                        </item-actions>
                      </item>
                      <item>
                        <item-media variant="icon">
                          <spinner running="true" />
                        </item-media>
                        <item-content>
                          <item-title>{copy.deployment_edge}</item-title>
                          <item-description>
                            {copy.deployment_edge_desc}
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">
                            {copy.deployment_percent}
                          </Text>
                        </item-actions>
                      </item>
                    </div>
                    <alert variant="warning">
                      <icon name="warning" />
                      <alert-content>
                        <alert-title>
                          {copy.deployment_warning_title}
                        </alert-title>
                        <alert-description>
                          {copy.deployment_warning_desc}
                        </alert-description>
                      </alert-content>
                    </alert>
                    <div className="home-row home-deploy-actions">
                      <button-group>
                        <Button variant="outline">
                          <Text>{copy.deployment_view_logs}</Text>
                        </Button>
                        <dropdown-menu>
                          <dropdown-menu-trigger>
                            <Button variant="outline" size="icon">
                              <icon name="chevron-down" />
                            </Button>
                          </dropdown-menu-trigger>
                          <dropdown-menu-content align="end">
                            <dropdown-menu-item>
                              <Text>{copy.deployment_rollback}</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>{copy.deployment_copy_hash}</Text>
                            </dropdown-menu-item>
                          </dropdown-menu-content>
                        </dropdown-menu>
                      </button-group>
                      <Button
                        variant="primary"
                        className="home-deploy-redeploy"
                      >
                        <icon name="arrow-sync" />
                        <Text>{copy.deployment_redeploy}</Text>
                      </Button>
                    </div>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-empty">
                    <empty>
                      <empty-header>
                        <div className="home-empty-icon">
                          <icon name="add" />
                        </div>
                        <empty-title>{copy.distribute_title}</empty-title>
                        <empty-description>
                          {copy.distribute_description}
                        </empty-description>
                      </empty-header>
                      <empty-content>
                        <Button variant="primary">
                          {copy.distribute_create}
                        </Button>
                      </empty-content>
                    </empty>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-claim">
                    <Text className="home-claim-label">{copy.claim_label}</Text>
                    <Text className="home-balance">{copy.claim_balance}</Text>
                    <div className="home-row">
                      <badge variant="outline">
                        <div className="home-dot" />
                        <Text>{copy.claim_pending}</Text>
                      </badge>
                    </div>
                    <div className="home-summary">
                      <div className="home-row home-row-between">
                        <Text className="home-summary-label">
                          {copy.claim_net_label}
                        </Text>
                        <Text className="home-summary-value">
                          {copy.claim_net}
                        </Text>
                      </div>
                      <div className="home-row home-row-between">
                        <Text className="home-summary-label">
                          {copy.claim_fee_label}
                        </Text>
                        <Text className="home-summary-value">
                          {copy.claim_fee}
                        </Text>
                      </div>
                      <div className="home-divider" />
                      <div className="home-row home-row-between">
                        <Text className="home-summary-total">
                          {copy.claim_total_label}
                        </Text>
                        <Text className="home-summary-total">
                          {copy.claim_total}
                        </Text>
                      </div>
                    </div>
                    <Text className="home-preview-desc">
                      {copy.claim_description}
                    </Text>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-payout clip-safe">
                    <div className="home-row home-row-between">
                      <card-title>{copy.payout_title}</card-title>
                      <Button variant="ghost" size="icon-sm">
                        <icon name="dismiss" />
                      </Button>
                    </div>
                    <card-description>
                      {copy.payout_description}
                    </card-description>
                    <field>
                      <field-label>{copy.payout_currency_label}</field-label>
                      <dropdown-menu>
                        <dropdown-menu-trigger>
                          <Button variant="outline" className="home-select">
                            <Text>{copy.payout_currency}</Text>
                            <icon name="chevron-down" />
                          </Button>
                        </dropdown-menu-trigger>
                        <dropdown-menu-content align="start">
                          <dropdown-menu-radio-group
                            value={copy.payout_currency_value}
                          >
                            <dropdown-menu-radio-item value="usd">
                              <Text>{copy.payout_usd}</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="cny">
                              <Text>{copy.payout_cny}</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="jpy">
                              <Text>{copy.payout_jpy}</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="eur">
                              <Text>{copy.payout_eur}</Text>
                            </dropdown-menu-radio-item>
                          </dropdown-menu-radio-group>
                        </dropdown-menu-content>
                      </dropdown-menu>
                    </field>
                    <field>
                      <field-label>{copy.payout_min_label}</field-label>
                      <Text className="home-payout-amount">
                        {copy.payout_amount}
                      </Text>
                      <div className="home-slider">
                        <div className="home-slider-fill" />
                      </div>
                      <div className="home-row home-row-between">
                        <Text className="home-goal-meta">
                          {copy.payout_min}
                        </Text>
                        <Text className="home-goal-meta">
                          {copy.payout_max}
                        </Text>
                      </div>
                    </field>
                    <field>
                      <field-label>{copy.payout_notes}</field-label>
                      <textinput
                        className="home-textarea"
                        multiline="true"
                        placeholder={copy.payout_note_placeholder}
                      />
                    </field>
                    <Button variant="primary" className="home-block-button">
                      {copy.payout_save}
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-accordion">
                    <card-header>
                      <card-title>{copy.subscription_title}</card-title>
                      <card-description>
                        {copy.subscription_description}
                      </card-description>
                    </card-header>
                    <accordion>
                      <accordion-item value="plans" open="true">
                        <accordion-trigger>
                          <Text>{copy.subscription_q1}</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>{copy.subscription_a1}</Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="billing">
                        <accordion-trigger>
                          <Text>{copy.subscription_q2}</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>{copy.subscription_a2}</Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="cancel">
                        <accordion-trigger>
                          <Text>{copy.subscription_q3}</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>{copy.subscription_a3}</Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="change">
                        <accordion-trigger>
                          <Text>{copy.subscription_q4}</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>{copy.subscription_a4}</Text>
                        </accordion-content>
                      </accordion-item>
                    </accordion>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-milestone clip-safe">
                    <card-header>
                      <card-title>{copy.milestone_title}</card-title>
                      <card-description>
                        {copy.milestone_description}
                      </card-description>
                    </card-header>
                    <field>
                      <field-label>{copy.milestone_goal}</field-label>
                      <textinput
                        className="home-input"
                        placeholder={copy.milestone_goal_placeholder}
                      />
                    </field>
                    <div className="home-field-row clip-safe">
                      <field className="home-field-half clip-safe">
                        <field-label>
                          {copy.milestone_target_amount}
                        </field-label>
                        <textinput
                          className="home-input"
                          placeholder={copy.milestone_target_placeholder}
                        />
                      </field>
                      <field className="home-field-half clip-safe">
                        <field-label>{copy.milestone_target_date}</field-label>
                        <textinput
                          className="home-input"
                          placeholder={copy.milestone_date_placeholder}
                        />
                      </field>
                    </div>
                    <Button variant="primary" className="home-block-button">
                      {copy.milestone_create}
                    </Button>
                    <Button variant="outline" className="home-block-button">
                      {copy.milestone_cancel}
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-alerts">
                    <card-header>
                      <card-title>{copy.notifications_title}</card-title>
                      <card-description>
                        {copy.notifications_description}
                      </card-description>
                    </card-header>
                    <div className="home-alert-row">
                      <checkbox checked="true" />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          {copy.notifications_transaction}
                        </Text>
                        <Text className="home-alert-desc">
                          {copy.notifications_transaction_desc}
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox checked="true" />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          {copy.notifications_security}
                        </Text>
                        <Text className="home-alert-desc">
                          {copy.notifications_security_desc}
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          {copy.notifications_milestones}
                        </Text>
                        <Text className="home-alert-desc">
                          {copy.notifications_milestones_desc}
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          {copy.notifications_market}
                        </Text>
                        <Text className="home-alert-desc">
                          {copy.notifications_market_desc}
                        </Text>
                      </div>
                    </div>
                    <Button variant="primary" className="home-block-button">
                      {copy.notifications_save}
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-shortcuts">
                    <breadcrumb path={copy.shortcuts_breadcrumb} />
                    <item variant="muted" interactive="true">
                      <item-media variant="icon">
                        <icon name="settings" />
                      </item-media>
                      <item-content>
                        <item-title>{copy.shortcuts_change_limit}</item-title>
                        <item-description>
                          {copy.shortcuts_change_limit_desc}
                        </item-description>
                      </item-content>
                      <item-actions>
                        <icon name="chevron-right" />
                      </item-actions>
                    </item>
                    <item variant="muted" interactive="true">
                      <item-media variant="icon">
                        <icon name="calendar-agenda" />
                      </item-media>
                      <item-content>
                        <item-title>{copy.shortcuts_scheduled}</item-title>
                        <item-description>
                          {copy.shortcuts_scheduled_desc}
                        </item-description>
                      </item-content>
                      <item-actions>
                        <icon name="chevron-right" />
                      </item-actions>
                    </item>
                    <item variant="muted" interactive="true">
                      <item-media variant="icon">
                        <icon name="arrow-clockwise" />
                      </item-media>
                      <item-content>
                        <item-title>{copy.shortcuts_recurring}</item-title>
                        <item-description>
                          {copy.shortcuts_recurring_desc}
                        </item-description>
                      </item-content>
                      <item-actions>
                        <icon name="chevron-right" />
                      </item-actions>
                    </item>
                  </card-content>
                </card>
                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-account clip-safe">
                    <card-header>
                      <card-title>{copy.account_title}</card-title>
                      <card-description>
                        {copy.account_description}
                      </card-description>
                    </card-header>
                    <field>
                      <field-label>{copy.account_email}</field-label>
                      <textinput
                        className="home-input"
                        placeholder="artist@studio.inc"
                      />
                    </field>
                    <field>
                      <div className="home-row home-row-between">
                        <field-label>{copy.account_password}</field-label>
                        <Button
                          variant="link"
                          size="xs"
                          className="home-field-link"
                        >
                          {copy.account_forgot}
                        </Button>
                      </div>
                      <textinput
                        className="home-input"
                        type="password"
                        placeholder="********"
                      />
                    </field>
                    <Button variant="primary" className="home-block-button">
                      <icon name="lock-closed" />
                      <Text>{copy.account_update}</Text>
                    </Button>
                    <item
                      variant="muted"
                      interactive="true"
                      className="home-danger"
                    >
                      <item-media variant="icon">
                        <icon name="error-circle" />
                      </item-media>
                      <item-content>
                        <item-title>{copy.account_danger}</item-title>
                        <item-description>
                          {copy.account_danger_desc}
                        </item-description>
                      </item-content>
                      <item-actions>
                        <icon name="chevron-right" />
                      </item-actions>
                    </item>
                  </card-content>
                </card>
              </home-grid>
            </div>
          </div>
        </ScrollAreaContent>
        <Scrollbar direction="vertical" />
      </ScrollArea>
    </div>
  );
};

HomeGallery.shouldPreRender = true;

export default HomeGallery;
