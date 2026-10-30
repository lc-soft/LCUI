import {
  Breadcrumb,
  BreadcrumbItem,
  BreadcrumbLink,
  BreadcrumbList,
  BreadcrumbSeparator,
  Button,
} from "@lcui/react";

export default function Example() {
  return (
    <Breadcrumb className="breadcrumb-custom-demo">
      <BreadcrumbList>
        <BreadcrumbItem data-path="/home">
          <BreadcrumbLink>Home</BreadcrumbLink>
          <BreadcrumbSeparator />
        </BreadcrumbItem>
        <BreadcrumbItem data-path="/home/components">
          <BreadcrumbLink>Components</BreadcrumbLink>
          <BreadcrumbSeparator />
        </BreadcrumbItem>
        <BreadcrumbItem data-key="settings" data-label="Settings">
          <Button variant="ghost" size="sm">
            Settings
          </Button>
        </BreadcrumbItem>
      </BreadcrumbList>
    </Breadcrumb>
  );
}
