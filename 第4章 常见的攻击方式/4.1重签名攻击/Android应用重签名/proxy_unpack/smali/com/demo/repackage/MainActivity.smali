.class public Lcom/demo/repackage/MainActivity;
.super Landroid/app/Activity;
.source "MainActivity.java"


# direct methods
.method public constructor <init>()V
    .locals 0

    .line 9
    invoke-direct {p0}, Landroid/app/Activity;-><init>()V

    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 1

    .line 13
    invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V

    const p1, 0x7f0b001c

    .line 14
    invoke-virtual {p0, p1}, Lcom/demo/repackage/MainActivity;->setContentView(I)V

    .line 15
    invoke-virtual {p0}, Lcom/demo/repackage/MainActivity;->getApplicationContext()Landroid/content/Context;

    move-result-object p1

    invoke-static {p1}, Lcom/demo/tools/ToolUtils;->getSignature2(Landroid/content/Context;)V

    .line 17
    invoke-virtual {p0}, Lcom/demo/repackage/MainActivity;->getApplicationContext()Landroid/content/Context;

    move-result-object p1

    invoke-static {p1}, Lcom/demo/tools/ToolUtils;->checkSignature(Landroid/content/Context;)Z

    move-result p1

    const-string v0, "checkSignature"

    if-eqz p1, :cond_0

    const-string p1, "True"

    .line 18
    invoke-static {v0, p1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    goto :goto_0

    :cond_0
    const-string p1, "False"

    .line 20
    invoke-static {v0, p1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    :goto_0
    return-void
.end method
