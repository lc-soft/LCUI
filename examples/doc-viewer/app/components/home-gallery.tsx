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

const HomeGallery: FunctionWidget = ({ locale }: { locale: Locale }) => {
  const cta = ctaLabel[locale];

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
                        <Text>Button</Text>
                        <icon name="arrow-right" />
                      </Button>
                      <Button variant="secondary" size="sm">
                        Secondary
                      </Button>
                      <Button variant="outline" size="sm">
                        Outline
                      </Button>
                    </div>
                    <input-group>
                      <textinput
                        className="input-group-input"
                        placeholder="Name"
                      />
                      <input-group-addon align="inline-end">
                        <icon name="search" />
                      </input-group-addon>
                    </input-group>
                    <textinput
                      className="home-textarea"
                      multiline="true"
                      placeholder="Message"
                    />
                    <div className="home-row home-row-between">
                      <div className="home-row">
                        <badge variant="default">Badge</badge>
                        <badge variant="secondary">Secondary</badge>
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
                          <Button variant="outline">Alert Dialog</Button>
                        </alert-dialog-trigger>
                        <alert-dialog-overlay>
                          <alert-dialog-content>
                            <alert-dialog-header>
                              <alert-dialog-title>
                                Allow accessory to connect?
                              </alert-dialog-title>
                              <alert-dialog-description>
                                Do you want to allow the USB accessory to
                                connect to this device and your data?
                              </alert-dialog-description>
                            </alert-dialog-header>
                            <alert-dialog-footer>
                              <alert-dialog-cancel variant="outline">
                                Don't allow
                              </alert-dialog-cancel>
                              <alert-dialog-action variant="primary">
                                Allow
                              </alert-dialog-action>
                            </alert-dialog-footer>
                          </alert-dialog-content>
                        </alert-dialog-overlay>
                      </alert-dialog>
                      <button-group>
                        <Button variant="outline">Button Group</Button>
                        <dropdown-menu>
                          <dropdown-menu-trigger>
                            <Button variant="outline" size="icon">
                              <icon name="chevron-down" />
                            </Button>
                          </dropdown-menu-trigger>
                          <dropdown-menu-content align="end">
                            <dropdown-menu-label>
                              <Text>Quick Actions</Text>
                            </dropdown-menu-label>
                            <dropdown-menu-item>
                              <Text>Mute Conversation</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>Mark as Read</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>Block User</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-separator />
                            <dropdown-menu-item variant="destructive">
                              <Text>Delete Conversation</Text>
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
                      <card-title>Attachments</card-title>
                      <card-description>
                        Upload, review, and manage your files.
                      </card-description>
                    </card-header>
                    <attachment state="idle">
                      <attachment-media>
                        <icon name="clock" />
                      </attachment-media>
                      <attachment-content>
                        <attachment-title>selected-file.pdf</attachment-title>
                        <attachment-description>
                          Ready to upload
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
                          Uploading - 64%
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
                          Processing document
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
                          Upload failed. Try again.
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
                          Uploaded - 1.8 MB
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
                      <card-title>Savings Targets</card-title>
                      <card-description>
                        Active milestones for 2024 across your portfolio.
                        Monitor how close you are to each savings goal.
                      </card-description>
                    </card-header>
                    <div className="home-goal">
                      <Text className="home-goal-label">RETIREMENT</Text>
                      <Text className="home-goal-value">$420,000</Text>
                      <progress value="65" className="home-goal-progress" />
                      <div className="home-goal-footer">
                        <Text className="home-goal-caption">65% achieved</Text>
                        <Text className="home-goal-amount">$273,000</Text>
                      </div>
                    </div>
                    <div className="home-goal">
                      <Text className="home-goal-label">REAL ESTATE</Text>
                      <Text className="home-goal-value">$85,000</Text>
                      <progress value="32" className="home-goal-progress" />
                      <div className="home-goal-footer">
                        <Text className="home-goal-caption">32% achieved</Text>
                        <Text className="home-goal-amount">$27,200</Text>
                      </div>
                    </div>
                    <Text className="home-preview-desc">
                      You have not met your targets for this year.
                    </Text>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-claim">
                    <Text className="home-claim-label">Claimable Balance</Text>
                    <Text className="home-balance">$1,211.29</Text>
                    <div className="home-row">
                      <badge variant="outline">
                        <div className="home-dot" />
                        <Text>Pending Setup</Text>
                      </badge>
                    </div>
                    <div className="home-summary">
                      <div className="home-row home-row-between">
                        <Text className="home-summary-label">
                          Net Royalties
                        </Text>
                        <Text className="home-summary-value">$1,248.75</Text>
                      </div>
                      <div className="home-row home-row-between">
                        <Text className="home-summary-label">
                          Processing Fee
                        </Text>
                        <Text className="home-summary-value">-$37.46</Text>
                      </div>
                      <div className="home-divider" />
                      <div className="home-row home-row-between">
                        <Text className="home-summary-total">
                          Total Ready to Claim
                        </Text>
                        <Text className="home-summary-total">
                          $1,211.29 USD
                        </Text>
                      </div>
                    </div>
                    <Text className="home-preview-desc">
                      Once your bank is connected, balances over $10 are
                      automatically eligible for monthly distribution on the
                      15th of each month.
                    </Text>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-accordion">
                    <card-header>
                      <card-title>Subscription & Billing</card-title>
                      <card-description>
                        Common questions about your account, plans, payments and
                        cancellations.
                      </card-description>
                    </card-header>
                    <accordion>
                      <accordion-item value="plans" open="true">
                        <accordion-trigger>
                          <Text>What subscription plans do you offer?</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>
                            We offer three subscription tiers: Starter
                            ($9/month), Professional ($29/month), and Enterprise
                            ($99/month). Each plan includes increasing storage
                            limits, API access, priority support, and team
                            collaboration features.
                          </Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="billing">
                        <accordion-trigger>
                          <Text>How does billing work?</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>
                            Billing occurs automatically at the start of each
                            billing cycle. We accept all major credit cards,
                            PayPal, and ACH transfers for enterprise customers.
                          </Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="cancel">
                        <accordion-trigger>
                          <Text>How do I cancel my subscription?</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>
                            You can cancel your subscription anytime from your
                            account settings. There are no cancellation fees or
                            penalties.
                          </Text>
                        </accordion-content>
                      </accordion-item>
                      <accordion-item value="change">
                        <accordion-trigger>
                          <Text>Can I change plans later?</Text>
                        </accordion-trigger>
                        <accordion-content>
                          <Text>
                            Yes. Upgrades take effect immediately and downgrades
                            apply at the end of the current billing period.
                          </Text>
                        </accordion-content>
                      </accordion-item>
                    </accordion>
                  </card-content>
                </card>

                <card className="home-card" data-column="2">
                  <card-content className="home-card-body home-deployment">
                    <card-header>
                      <div className="home-row home-row-between">
                        <card-title>Deployment Status</card-title>
                        <badge variant="outline">
                          <div className="home-deploy-live-dot" />
                          <Text>Live</Text>
                        </badge>
                      </div>
                      <card-description>
                        Production rollout for the documentation site.
                      </card-description>
                    </card-header>
                    <div className="home-deploy-progress">
                      <div className="home-row home-row-between">
                        <Text className="home-deploy-environment">
                          production
                        </Text>
                        <Text className="home-deploy-percent">82%</Text>
                      </div>
                      <progress value="82" />
                    </div>
                    <div className="home-deploy-steps">
                      <item>
                        <item-media variant="icon">
                          <icon name="checkmark" />
                        </item-media>
                        <item-content>
                          <item-title>Build & test</item-title>
                          <item-description>
                            Windows, Linux and macOS passed.
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">2m 14s</Text>
                        </item-actions>
                      </item>
                      <item>
                        <item-media variant="icon">
                          <icon name="checkmark" />
                        </item-media>
                        <item-content>
                          <item-title>Database migration</item-title>
                          <item-description>
                            Schema changes applied successfully.
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">38s</Text>
                        </item-actions>
                      </item>
                      <item>
                        <item-media variant="icon">
                          <spinner running="true" />
                        </item-media>
                        <item-content>
                          <item-title>Edge rollout</item-title>
                          <item-description>
                            Shifting traffic across regions.
                          </item-description>
                        </item-content>
                        <item-actions>
                          <Text className="home-deploy-step-meta">82%</Text>
                        </item-actions>
                      </item>
                    </div>
                    <alert variant="warning">
                      <icon name="warning" />
                      <alert-content>
                        <alert-title>One region is warming up</alert-title>
                        <alert-description>
                          Traffic is still being shifted in eu-west.
                        </alert-description>
                      </alert-content>
                    </alert>
                    <div className="home-row home-deploy-actions">
                      <button-group>
                        <Button variant="outline">
                          <Text>View logs</Text>
                        </Button>
                        <dropdown-menu>
                          <dropdown-menu-trigger>
                            <Button variant="outline" size="icon">
                              <icon name="chevron-down" />
                            </Button>
                          </dropdown-menu-trigger>
                          <dropdown-menu-content align="end">
                            <dropdown-menu-item>
                              <Text>Rollback release</Text>
                            </dropdown-menu-item>
                            <dropdown-menu-item>
                              <Text>Copy commit hash</Text>
                            </dropdown-menu-item>
                          </dropdown-menu-content>
                        </dropdown-menu>
                      </button-group>
                      <Button
                        variant="primary"
                        className="home-deploy-redeploy"
                      >
                        <icon name="arrow-sync" />
                        <Text>Redeploy</Text>
                      </Button>
                    </div>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-milestone clip-safe">
                    <card-header>
                      <card-title>Set a new milestone</card-title>
                      <card-description>
                        Define your financial target and we'll help you pace
                        your savings.
                      </card-description>
                    </card-header>
                    <field>
                      <field-label>Goal Name</field-label>
                      <textinput
                        className="home-input"
                        placeholder="e.g. New Car"
                      />
                    </field>
                    <div className="home-field-row clip-safe">
                      <field className="home-field-half clip-safe">
                        <field-label>Target Amount</field-label>
                        <textinput
                          className="home-input"
                          placeholder="$15,000"
                        />
                      </field>
                      <field className="home-field-half clip-safe">
                        <field-label>Target Date</field-label>
                        <textinput
                          className="home-input"
                          placeholder="Dec 2025"
                        />
                      </field>
                    </div>
                    <Button variant="primary" className="home-block-button">
                      Create Goal
                    </Button>
                    <Button variant="outline" className="home-block-button">
                      Cancel
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-payout clip-safe">
                    <div className="home-row home-row-between">
                      <card-title>Payout Threshold</card-title>
                      <Button variant="ghost" size="icon-sm">
                        <icon name="dismiss" />
                      </Button>
                    </div>
                    <card-description>
                      Set the minimum balance required before a payout is
                      triggered.
                    </card-description>
                    <field>
                      <field-label>Preferred Currency</field-label>
                      <dropdown-menu>
                        <dropdown-menu-trigger>
                          <Button variant="outline" className="home-select">
                            <Text>USD — United States Dollar</Text>
                            <icon name="chevron-down" />
                          </Button>
                        </dropdown-menu-trigger>
                        <dropdown-menu-content align="start">
                          <dropdown-menu-radio-group value="usd">
                            <dropdown-menu-radio-item value="usd">
                              <Text>USD — United States Dollar</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="cny">
                              <Text>CNY — Chinese Yuan</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="jpy">
                              <Text>JPY — Japanese Yen</Text>
                            </dropdown-menu-radio-item>
                            <dropdown-menu-radio-item value="eur">
                              <Text>EUR — Euro</Text>
                            </dropdown-menu-radio-item>
                          </dropdown-menu-radio-group>
                        </dropdown-menu-content>
                      </dropdown-menu>
                    </field>
                    <field>
                      <field-label>Minimum Payout Amount</field-label>
                      <Text className="home-payout-amount">$2500.00</Text>
                      <div className="home-slider">
                        <div className="home-slider-fill" />
                      </div>
                      <div className="home-row home-row-between">
                        <Text className="home-goal-meta">$50 (MIN)</Text>
                        <Text className="home-goal-meta">$10,000 (MAX)</Text>
                      </div>
                    </field>
                    <field>
                      <field-label>Notes</field-label>
                      <textinput
                        className="home-textarea"
                        multiline="true"
                        placeholder="Add a note for this payout"
                      />
                    </field>
                    <Button variant="primary" className="home-block-button">
                      Save Threshold
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="3">
                  <card-content className="home-card-body home-account clip-safe">
                    <card-header>
                      <card-title>Account Access</card-title>
                      <card-description>
                        Update your credentials or re-authenticate.
                      </card-description>
                    </card-header>
                    <field>
                      <field-label>Email Address</field-label>
                      <textinput
                        className="home-input"
                        placeholder="artist@studio.inc"
                      />
                    </field>
                    <field>
                      <div className="home-row home-row-between">
                        <field-label>Current Password</field-label>
                        <Button
                          variant="link"
                          size="xs"
                          className="home-field-link"
                        >
                          FORGOT?
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
                      <Text>Update Security</Text>
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
                        <item-title>Danger Zone</item-title>
                        <item-description>
                          Archive account and remove…
                        </item-description>
                      </item-content>
                      <item-actions>
                        <icon name="chevron-right" />
                      </item-actions>
                    </item>
                  </card-content>
                </card>

                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-empty">
                    <empty>
                      <empty-header>
                        <div className="home-empty-icon">
                          <icon name="add" />
                        </div>
                        <empty-title>Distribute Track</empty-title>
                        <empty-description>
                          Upload your first master to start reaching listeners
                          on Spotify, Apple Music, and more.
                        </empty-description>
                      </empty-header>
                      <empty-content>
                        <Button variant="primary">Create Release</Button>
                      </empty-content>
                    </empty>
                  </card-content>
                </card>

                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-alerts">
                    <card-header>
                      <card-title>Notifications</card-title>
                      <card-description>
                        Choose which email and push alerts you want to receive.
                      </card-description>
                    </card-header>
                    <div className="home-alert-row">
                      <checkbox checked="true" />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          Transaction alerts
                        </Text>
                        <Text className="home-alert-desc">
                          Deposits, withdrawals, and transfers.
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox checked="true" />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          Security alerts
                        </Text>
                        <Text className="home-alert-desc">
                          Login attempts and account changes.
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">
                          Goal milestones
                        </Text>
                        <Text className="home-alert-desc">
                          Updates at 25%, 50%, 75%, and 100%.
                        </Text>
                      </div>
                    </div>
                    <div className="home-alert-row">
                      <checkbox />
                      <div className="home-alert-text">
                        <Text className="home-alert-title">Market updates</Text>
                        <Text className="home-alert-desc">
                          Daily portfolio summary and price alerts.
                        </Text>
                      </div>
                    </div>
                    <Button variant="primary" className="home-block-button">
                      Save Preferences
                    </Button>
                  </card-content>
                </card>

                <card className="home-card" data-column="4">
                  <card-content className="home-card-body home-shortcuts">
                    <breadcrumb path="/Home/Profile/Statements/Documents/Payments" />
                    <item variant="muted" interactive="true">
                      <item-media variant="icon">
                        <icon name="settings" />
                      </item-media>
                      <item-content>
                        <item-title>Change transfer limit</item-title>
                        <item-description>
                          Adjust how much you can send from your balance.
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
                        <item-title>Scheduled transfers</item-title>
                        <item-description>
                          Set up a transfer to send at a later date.
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
                        <item-title>Recurring card payments</item-title>
                        <item-description>
                          Manage your repeated card transactions.
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
