import {
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuLabel,
  DropdownMenuSeparator,
  DropdownMenuTrigger,
  Icon,
  ScrollArea,
  ScrollAreaContent,
  Scrollbar,
  Sidebar,
  SidebarContent,
  SidebarFooter,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarHeader,
  SidebarMenu,
  SidebarMenuAction,
  SidebarMenuButton,
  SidebarMenuItem,
  SidebarMenuSub,
  SidebarMenuSubButton,
  SidebarMenuSubItem,
  SidebarProvider,
  SidebarRail,
  SidebarTrigger,
  Text,
  Widget,
} from "@lcui/react";

const nav = [
  {
    title: "Playground",
    icon: "code-block",
    open: true,
    items: ["History", "Starred", "Settings"],
  },
  {
    title: "Models",
    icon: "document-text",
    items: ["Genesis", "Explorer", "Quantum"],
  },
  {
    title: "Documentation",
    icon: "book-open",
    items: ["Introduction", "Get Started", "Tutorials", "Changelog"],
  },
  {
    title: "Settings",
    icon: "calendar-settings",
    items: ["General", "Team", "Billing", "Limits"],
  },
];

const projects = [
  { title: "Design Engineering", icon: "code-block" },
  { title: "Sales & Marketing", icon: "book-open" },
  { title: "Travel", icon: "document-text" },
];

export default function App() {
  return (
    <SidebarProvider className="flex h-[500px] w-full rounded-lg border">
      <Sidebar collapsible="icon">
        <SidebarHeader>
          <SidebarMenu>
            <SidebarMenuItem>
              <DropdownMenu className="sidebar-team-menu">
                <DropdownMenuTrigger>
                  <SidebarMenuButton
                    className="sidebar-team h-12 p-2"
                    data-sidebar-tooltip="Acme Inc"
                  >
                    <Widget className="sidebar-team-logo">
                      <Icon name="people-team" />
                    </Widget>
                    <Widget className="flex flex-1 flex-col gap-0.5 text-left">
                      <Text className="text-sm font-medium">Acme Inc</Text>
                      <Text className="text-xs text-muted-foreground">
                        Enterprise
                      </Text>
                    </Widget>
                    <Icon
                      className="sidebar-menu-chevron"
                      name="chevron-up-down"
                    />
                  </SidebarMenuButton>
                </DropdownMenuTrigger>
                <DropdownMenuContent className="w-56" side="right" align="start">
                  <DropdownMenuLabel>Teams</DropdownMenuLabel>
                  <DropdownMenuItem>
                    <Icon name="people-team" />
                    <Text>Acme Inc</Text>
                  </DropdownMenuItem>
                  <DropdownMenuItem>
                    <Icon name="building" />
                    <Text>Acme Corp</Text>
                  </DropdownMenuItem>
                  <DropdownMenuItem>
                    <Icon name="shield" />
                    <Text>Evil Corp</Text>
                  </DropdownMenuItem>
                  <DropdownMenuSeparator />
                  <DropdownMenuItem>
                    <Icon name="add" />
                    <Text>Add team</Text>
                  </DropdownMenuItem>
                </DropdownMenuContent>
              </DropdownMenu>
            </SidebarMenuItem>
          </SidebarMenu>
        </SidebarHeader>
        <SidebarContent>
          <ScrollArea className="sidebar-scroll">
            <ScrollAreaContent>
              <SidebarGroup>
                <SidebarGroupLabel>Platform</SidebarGroupLabel>
                <SidebarGroupContent>
                  <SidebarMenu>
                    {nav.map((item) => (
                      <Collapsible key={item.title} defaultOpen={item.open}>
                        <SidebarMenuItem>
                          <CollapsibleTrigger>
                            <SidebarMenuButton
                              data-sidebar-tooltip={item.title}
                            >
                              <Icon name={item.icon} />
                              <Text className="sidebar-menu-label">
                                {item.title}
                              </Text>
                              <Icon
                                className="sidebar-menu-chevron"
                                name="chevron-right"
                              />
                            </SidebarMenuButton>
                          </CollapsibleTrigger>
                          <CollapsibleContent>
                            <SidebarMenuSub>
                              {item.items.map((sub) => (
                                <SidebarMenuSubItem key={sub}>
                                  <SidebarMenuSubButton>
                                    <Text>{sub}</Text>
                                  </SidebarMenuSubButton>
                                </SidebarMenuSubItem>
                              ))}
                            </SidebarMenuSub>
                          </CollapsibleContent>
                        </SidebarMenuItem>
                      </Collapsible>
                    ))}
                  </SidebarMenu>
                </SidebarGroupContent>
              </SidebarGroup>
              <SidebarGroup className="sidebar-projects">
                <SidebarGroupLabel>Projects</SidebarGroupLabel>
                <SidebarGroupContent>
                  <SidebarMenu>
                    {projects.map((project) => (
                      <SidebarMenuItem key={project.title}>
                        <SidebarMenuButton>
                          <Icon name={project.icon} />
                          <Text className="sidebar-menu-label">
                            {project.title}
                          </Text>
                        </SidebarMenuButton>
                        <DropdownMenu>
                          <DropdownMenuTrigger className="sidebar-menu-action-trigger">
                            <SidebarMenuAction>
                              <Icon name="more-horizontal" />
                            </SidebarMenuAction>
                          </DropdownMenuTrigger>
                          <DropdownMenuContent side="right" align="start">
                            <DropdownMenuItem>
                              <Icon name="folder" />
                              <Text>View Project</Text>
                            </DropdownMenuItem>
                            <DropdownMenuItem>
                              <Icon name="arrow-right" />
                              <Text>Share Project</Text>
                            </DropdownMenuItem>
                            <DropdownMenuSeparator />
                            <DropdownMenuItem variant="destructive">
                              <Icon name="delete" />
                              <Text>Delete Project</Text>
                            </DropdownMenuItem>
                          </DropdownMenuContent>
                        </DropdownMenu>
                      </SidebarMenuItem>
                    ))}
                    <SidebarMenuItem>
                      <SidebarMenuButton className="sidebar-more">
                        <Icon name="more-horizontal" />
                        <Text className="sidebar-menu-label">More</Text>
                      </SidebarMenuButton>
                    </SidebarMenuItem>
                  </SidebarMenu>
                </SidebarGroupContent>
              </SidebarGroup>
            </ScrollAreaContent>
            <Scrollbar />
          </ScrollArea>
        </SidebarContent>
        <SidebarFooter>
          <SidebarMenu>
            <SidebarMenuItem>
              <DropdownMenu className="sidebar-user-menu">
                <DropdownMenuTrigger>
                  <SidebarMenuButton
                    className="sidebar-user h-11 p-2"
                    data-sidebar-tooltip="shadcn"
                  >
                    <Widget className="sidebar-avatar">
                      <Icon name="person-circle" />
                    </Widget>
                    <Widget className="flex flex-1 flex-col gap-0.5 text-left">
                      <Text className="text-sm font-medium">shadcn</Text>
                      <Text className="text-xs text-muted-foreground">
                        m@example.com
                      </Text>
                    </Widget>
                    <Icon
                      className="sidebar-menu-chevron"
                      name="chevron-up-down"
                    />
                  </SidebarMenuButton>
                </DropdownMenuTrigger>
                <DropdownMenuContent
                  className="w-56"
                  side="right"
                  align="end"
                >
                  <DropdownMenuLabel>shadcn</DropdownMenuLabel>
                  <Text className="px-2 pb-1.5 text-xs text-muted-foreground">
                    m@example.com
                  </Text>
                  <DropdownMenuSeparator />
                  <DropdownMenuItem>
                    <Icon name="premium" />
                    <Text>Upgrade to Pro</Text>
                  </DropdownMenuItem>
                  <DropdownMenuSeparator />
                  <DropdownMenuItem>
                    <Icon name="person" />
                    <Text>Account</Text>
                  </DropdownMenuItem>
                  <DropdownMenuItem>
                    <Icon name="payment" />
                    <Text>Billing</Text>
                  </DropdownMenuItem>
                  <DropdownMenuItem>
                    <Icon name="alert" />
                    <Text>Notifications</Text>
                  </DropdownMenuItem>
                  <DropdownMenuSeparator />
                  <DropdownMenuItem>
                    <Icon name="sign-out" />
                    <Text>Sign Out</Text>
                  </DropdownMenuItem>
                </DropdownMenuContent>
              </DropdownMenu>
            </SidebarMenuItem>
          </SidebarMenu>
        </SidebarFooter>
        <SidebarRail />
      </Sidebar>
      <Widget className="flex flex-1 flex-col gap-2 p-3">
        <SidebarTrigger />
      </Widget>
    </SidebarProvider>
  );
}
