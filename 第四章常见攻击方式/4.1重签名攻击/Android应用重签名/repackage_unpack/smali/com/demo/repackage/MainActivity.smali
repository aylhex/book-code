.class public Lcom/demo/repackage/MainActivity;
.super Landroidx/appcompat/app/AppCompatActivity;
.source "MainActivity.java"


# instance fields
.field private binding:Lcom/demo/repackage/databinding/ActivityMainBinding;


# direct methods
.method public constructor <init>()V
    .locals 0

    .line 24
    invoke-direct {p0}, Landroidx/appcompat/app/AppCompatActivity;-><init>()V

    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 2

    .line 30
    invoke-super {p0, p1}, Landroidx/appcompat/app/AppCompatActivity;->onCreate(Landroid/os/Bundle;)V

    .line 32
    invoke-virtual {p0}, Lcom/demo/repackage/MainActivity;->getLayoutInflater()Landroid/view/LayoutInflater;

    move-result-object p1

    invoke-static {p1}, Lcom/demo/repackage/databinding/ActivityMainBinding;->inflate(Landroid/view/LayoutInflater;)Lcom/demo/repackage/databinding/ActivityMainBinding;

    move-result-object p1

    iput-object p1, p0, Lcom/demo/repackage/MainActivity;->binding:Lcom/demo/repackage/databinding/ActivityMainBinding;

    .line 33
    invoke-virtual {p1}, Lcom/demo/repackage/databinding/ActivityMainBinding;->getRoot()Landroidx/coordinatorlayout/widget/CoordinatorLayout;

    move-result-object p1

    invoke-virtual {p0, p1}, Lcom/demo/repackage/MainActivity;->setContentView(Landroid/view/View;)V

    .line 35
    iget-object p1, p0, Lcom/demo/repackage/MainActivity;->binding:Lcom/demo/repackage/databinding/ActivityMainBinding;

    iget-object p1, p1, Lcom/demo/repackage/databinding/ActivityMainBinding;->toolbar:Landroidx/appcompat/widget/Toolbar;

    invoke-virtual {p0, p1}, Lcom/demo/repackage/MainActivity;->setSupportActionBar(Landroidx/appcompat/widget/Toolbar;)V

    .line 37
    invoke-virtual {p0}, Lcom/demo/repackage/MainActivity;->getApplicationContext()Landroid/content/Context;

    move-result-object p1

    invoke-static {p1}, Lcom/demo/repackage/ToolUtils;->getAppSignature(Landroid/content/Context;)Ljava/lang/String;

    move-result-object p1

    const-string v0, "current sign"

    invoke-static {v0, p1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 39
    invoke-virtual {p0}, Lcom/demo/repackage/MainActivity;->getApplicationContext()Landroid/content/Context;

    move-result-object p1

    invoke-static {p1}, Lcom/demo/repackage/ToolUtils;->checkSignature(Landroid/content/Context;)Z

    move-result p1

    if-nez p1, :cond_0

    .line 40
    new-instance p1, Landroidx/appcompat/app/AlertDialog$Builder;

    invoke-direct {p1, p0}, Landroidx/appcompat/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    const-string v0, "\u5b89\u5168\u63d0\u9192"

    invoke-virtual {p1, v0}, Landroidx/appcompat/app/AlertDialog$Builder;->setTitle(Ljava/lang/CharSequence;)Landroidx/appcompat/app/AlertDialog$Builder;

    move-result-object p1

    const-string v0, "\u5f53\u524d\u4f7f\u7528\u7684\u5ba2\u6237\u7aef\u4e3a\u975e\u5b98\u65b9\u7248\u672c\uff0c\u5b58\u5728\u6781\u5927\u7684\u5b89\u5168\u9690\u60a3\u3002\u4e3a\u4e86\u60a8\u7684\u8d22\u4ea7\u5b89\u5168\u8bf7\u5728\u5b98\u65b9\u6e20\u9053\u4e0b\u8f7d\u91cd\u65b0\u5b89\u88c5\uff01"

    .line 42
    invoke-virtual {p1, v0}, Landroidx/appcompat/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroidx/appcompat/app/AlertDialog$Builder;

    move-result-object p1

    new-instance v0, Lcom/demo/repackage/MainActivity$1;

    invoke-direct {v0, p0}, Lcom/demo/repackage/MainActivity$1;-><init>(Lcom/demo/repackage/MainActivity;)V

    const-string v1, "\u786e\u5b9a"

    .line 43
    invoke-virtual {p1, v1, v0}, Landroidx/appcompat/app/AlertDialog$Builder;->setPositiveButton(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroidx/appcompat/app/AlertDialog$Builder;

    move-result-object p1

    .line 49
    invoke-virtual {p1}, Landroidx/appcompat/app/AlertDialog$Builder;->show()Landroidx/appcompat/app/AlertDialog;

    :cond_0
    return-void
.end method
