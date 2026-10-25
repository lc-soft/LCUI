import {
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
    <SidebarProvider
      defaultOpen={false}
      className="flex h-[360px] w-full rounded-lg border"
    >
      <Sidebar collapsible="icon">
        <SidebarContent>
          <SidebarGroup>
            <SidebarGroupLabel>Application</SidebarGroupLabel>
            <SidebarGroupContent>
              <SidebarMenu>
                <SidebarMenuItem>
                  <SidebarMenuButton>
                    <Icon name="book-open" />
                    <Text className="sidebar-menu-label">Home</Text>
                  </SidebarMenuButton>
                </SidebarMenuItem>
                <SidebarMenuItem>
                  <SidebarMenuButton>
                    <Icon name="calendar-mail" />
                    <Text className="sidebar-menu-label">Inbox</Text>
                  </SidebarMenuButton>
                </SidebarMenuItem>
              </SidebarMenu>
            </SidebarGroupContent>
          </SidebarGroup>
        </SidebarContent>
      </Sidebar>
      <Widget className="flex flex-1 flex-col gap-2 p-3">
        <SidebarTrigger />
        <Text className="text-base font-semibold">Dashboard</Text>
        <Text className="text-sm text-muted-foreground">
          Click the trigger to expand the sidebar.
        </Text>
      </Widget>
    </SidebarProvider>
  );
}
