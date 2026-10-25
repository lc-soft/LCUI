import {
  Collapsible,
  CollapsibleContent,
  CollapsibleTrigger,
  Icon,
  Sidebar,
  SidebarContent,
  SidebarGroup,
  SidebarGroupContent,
  SidebarGroupLabel,
  SidebarMenu,
  SidebarMenuButton,
  SidebarMenuItem,
  SidebarProvider,
  SidebarTrigger,
  Text,
  Widget,
} from "@lcui/react";

export default function App() {
  return (
    <SidebarProvider className="flex h-[360px] w-full rounded-lg border">
      <Sidebar>
        <SidebarContent>
          <Collapsible defaultOpen>
            <SidebarGroup>
              <CollapsibleTrigger>
                <SidebarGroupLabel>
                  <Text>Help</Text>
                  <Icon name="chevron-down" />
                </SidebarGroupLabel>
              </CollapsibleTrigger>
              <CollapsibleContent>
                <SidebarGroupContent>
                  <SidebarMenu>
                    <SidebarMenuItem>
                      <SidebarMenuButton>
                        <Icon name="chat-help" />
                        <Text className="sidebar-menu-label">Support</Text>
                      </SidebarMenuButton>
                    </SidebarMenuItem>
                    <SidebarMenuItem>
                      <SidebarMenuButton>
                        <Icon name="calendar-mail" />
                        <Text className="sidebar-menu-label">Feedback</Text>
                      </SidebarMenuButton>
                    </SidebarMenuItem>
                  </SidebarMenu>
                </SidebarGroupContent>
              </CollapsibleContent>
            </SidebarGroup>
          </Collapsible>
        </SidebarContent>
      </Sidebar>
      <Widget className="flex flex-1 flex-col gap-2 p-3">
        <SidebarTrigger />
        <Text className="text-base font-semibold">Workspace</Text>
        <Text className="text-sm text-muted-foreground">
          Groups can collapse without leaving the sidebar.
        </Text>
      </Widget>
    </SidebarProvider>
  );
}
