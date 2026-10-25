import {
  Button,
  DropdownMenu,
  DropdownMenuCheckboxItem,
  DropdownMenuContent,
  DropdownMenuGroup,
  DropdownMenuItem,
  DropdownMenuLabel,
  DropdownMenuRadioGroup,
  DropdownMenuRadioItem,
  DropdownMenuSeparator,
  DropdownMenuShortcut,
  DropdownMenuSub,
  DropdownMenuSubContent,
  DropdownMenuSubTrigger,
  DropdownMenuTrigger,
  Icon,
  Text,
} from "@lcui/react";

export default function Example() {
  return (
    <DropdownMenu>
      <DropdownMenuTrigger>
        <Button>Complex Menu</Button>
      </DropdownMenuTrigger>
      <DropdownMenuContent className="w-44">
        <DropdownMenuGroup>
          <DropdownMenuLabel>File</DropdownMenuLabel>
          <DropdownMenuItem>
            <Icon name="document" />
            <Text>New File</Text>
            <DropdownMenuShortcut>Ctrl+N</DropdownMenuShortcut>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="folder" />
            <Text>New Folder</Text>
            <DropdownMenuShortcut>Shift+N</DropdownMenuShortcut>
          </DropdownMenuItem>
          <DropdownMenuSub>
            <DropdownMenuSubTrigger>
              <Icon name="folder-open" />
              <Text>Open Recent</Text>
              <Icon name="chevron-right" className="dropdown-menu-chevron" />
            </DropdownMenuSubTrigger>
            <DropdownMenuSubContent>
              <DropdownMenuLabel>Recent Projects</DropdownMenuLabel>
              <DropdownMenuItem>
                <Icon name="code" />
                <Text>Project Alpha</Text>
              </DropdownMenuItem>
              <DropdownMenuItem>
                <Icon name="code" />
                <Text>Project Beta</Text>
              </DropdownMenuItem>
              <DropdownMenuSub>
                <DropdownMenuSubTrigger>
                  <Icon name="more-horizontal" />
                  <Text>More Projects</Text>
                  <Icon name="chevron-right" className="dropdown-menu-chevron" />
                </DropdownMenuSubTrigger>
                <DropdownMenuSubContent>
                  <DropdownMenuItem>
                    <Icon name="code" />
                    <Text>Project Gamma</Text>
                  </DropdownMenuItem>
                  <DropdownMenuItem>
                    <Icon name="code" />
                    <Text>Project Delta</Text>
                  </DropdownMenuItem>
                </DropdownMenuSubContent>
              </DropdownMenuSub>
              <DropdownMenuSeparator />
              <DropdownMenuItem>
                <Icon name="search" />
                <Text>Browse...</Text>
              </DropdownMenuItem>
            </DropdownMenuSubContent>
          </DropdownMenuSub>
          <DropdownMenuSeparator />
          <DropdownMenuItem>
            <Icon name="save" />
            <Text>Save</Text>
            <DropdownMenuShortcut>Ctrl+S</DropdownMenuShortcut>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="arrow-download" />
            <Text>Export</Text>
            <DropdownMenuShortcut>Shift+E</DropdownMenuShortcut>
          </DropdownMenuItem>
        </DropdownMenuGroup>
        <DropdownMenuSeparator />
        <DropdownMenuGroup>
          <DropdownMenuLabel>View</DropdownMenuLabel>
          <DropdownMenuCheckboxItem checked>
            <Icon name="eye" />
            <Text>Show Sidebar</Text>
          </DropdownMenuCheckboxItem>
          <DropdownMenuCheckboxItem>
            <Icon name="grid" />
            <Text>Show Status Bar</Text>
          </DropdownMenuCheckboxItem>
          <DropdownMenuSub>
            <DropdownMenuSubTrigger>
              <Icon name="paint-brush" />
              <Text>Theme</Text>
              <Icon name="chevron-right" className="dropdown-menu-chevron" />
            </DropdownMenuSubTrigger>
            <DropdownMenuSubContent>
              <DropdownMenuLabel>Appearance</DropdownMenuLabel>
              <DropdownMenuRadioGroup value="light">
                <DropdownMenuRadioItem value="light">
                  <Icon name="weather-sunny" />
                  <Text>Light</Text>
                </DropdownMenuRadioItem>
                <DropdownMenuRadioItem value="dark">
                  <Icon name="weather-moon" />
                  <Text>Dark</Text>
                </DropdownMenuRadioItem>
                <DropdownMenuRadioItem value="system">
                  <Icon name="desktop" />
                  <Text>System</Text>
                </DropdownMenuRadioItem>
              </DropdownMenuRadioGroup>
            </DropdownMenuSubContent>
          </DropdownMenuSub>
        </DropdownMenuGroup>
        <DropdownMenuSeparator />
        <DropdownMenuGroup>
          <DropdownMenuLabel>Account</DropdownMenuLabel>
          <DropdownMenuItem>
            <Icon name="person" />
            <Text>Profile</Text>
            <DropdownMenuShortcut>Shift+P</DropdownMenuShortcut>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="payment" />
            <Text>Billing</Text>
          </DropdownMenuItem>
          <DropdownMenuSub>
            <DropdownMenuSubTrigger>
              <Icon name="settings" />
              <Text>Settings</Text>
              <Icon name="chevron-right" className="dropdown-menu-chevron" />
            </DropdownMenuSubTrigger>
            <DropdownMenuSubContent>
              <DropdownMenuLabel>Preferences</DropdownMenuLabel>
              <DropdownMenuItem>
                <Icon name="keyboard" />
                <Text>Keyboard Shortcuts</Text>
              </DropdownMenuItem>
              <DropdownMenuItem>
                <Icon name="local-language" />
                <Text>Language</Text>
              </DropdownMenuItem>
              <DropdownMenuSub>
                <DropdownMenuSubTrigger>
                  <Icon name="alert" />
                  <Text>Notifications</Text>
                  <Icon name="chevron-right" className="dropdown-menu-chevron" />
                </DropdownMenuSubTrigger>
                <DropdownMenuSubContent>
                  <DropdownMenuLabel>Notification Types</DropdownMenuLabel>
                  <DropdownMenuCheckboxItem checked>
                    <Icon name="alert" />
                    <Text>Push Notifications</Text>
                  </DropdownMenuCheckboxItem>
                  <DropdownMenuCheckboxItem checked>
                    <Icon name="mail" />
                    <Text>Email Notifications</Text>
                  </DropdownMenuCheckboxItem>
                </DropdownMenuSubContent>
              </DropdownMenuSub>
              <DropdownMenuSeparator />
              <DropdownMenuItem>
                <Icon name="shield" />
                <Text>Privacy &amp; Security</Text>
              </DropdownMenuItem>
            </DropdownMenuSubContent>
          </DropdownMenuSub>
        </DropdownMenuGroup>
        <DropdownMenuSeparator />
        <DropdownMenuGroup>
          <DropdownMenuItem>
            <Icon name="question-circle" />
            <Text>Help &amp; Support</Text>
          </DropdownMenuItem>
          <DropdownMenuItem>
            <Icon name="document-text" />
            <Text>Documentation</Text>
          </DropdownMenuItem>
        </DropdownMenuGroup>
        <DropdownMenuSeparator />
        <DropdownMenuGroup>
          <DropdownMenuItem variant="destructive">
            <Icon name="sign-out" />
            <Text>Sign Out</Text>
            <DropdownMenuShortcut>Shift+Q</DropdownMenuShortcut>
          </DropdownMenuItem>
        </DropdownMenuGroup>
      </DropdownMenuContent>
    </DropdownMenu>
  );
}
